#include "SteamTransport.h"

CSteamID SteamTransport::currentLobbyId;
CSteamID SteamTransport::localId;
uint32_t SteamTransport::localCounter = 0;

extern "C" {
    uint64_t SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex(void* instancePtr, uint64_t steamIDLobby, int iMember);
}

void SteamTransport::SendToAll(Packet packet, bool reliable)
{
    if (reliable)
        SendPacketToAll(packet);
    else
        SendUnreliablePacketToAll(packet);
}

void SteamTransport::ReceiveAll()
{
    RecievePackets();
}

void SteamTransport::HostLobby(int _maxMembers)
{
    maxMembers = _maxMembers;
    SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, maxMembers);
    m_LobbyCreatedCallResult.Set(hSteamAPICall, this, &SteamTransport::OnLobbyCreated);

    std::cout << "Hosted lobby | SteamID: " << localId.ConvertToUint64() << std::endl;
}

void SteamTransport::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
    if (bIOFailure || pCallback->m_eResult != k_EResultOK)
        return;

    currentLobbyId = CSteamID(pCallback->m_ulSteamIDLobby);
}

void SteamTransport::StartInviteOverlay()
{
    if (currentLobbyId.IsValid())
    {
        SteamFriends()->ActivateGameOverlayInviteDialog(currentLobbyId);
    }
}

void SteamTransport::OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* pCallback)
{
    JoinLobby(pCallback->m_steamIDLobby);
    std::cout << "Player with SteamID: " << localId.ConvertToUint64() << " requested to join lobby.\n";
}

void SteamTransport::OnSessionRequest(SteamNetworkingMessagesSessionRequest_t* pCallback)
{
    SteamNetworkingMessages()->AcceptSessionWithUser(pCallback->m_identityRemote);
}

// NEW: Check when Steam confirms a user has fully loaded their routing into the lobby
void SteamTransport::OnLobbyChatUpdate(LobbyChatUpdate_t* pCallback)
{
    if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeEntered)
    {
        CSteamID joinedID = pCallback->m_ulSteamIDUserChanged;
        
        if (OnPlayerJoinedLobbyEvent)
        {
            std::cout << "Player with SteamID: " << joinedID.ConvertToUint64() << " successfull joined lobby.\n";
            OnPlayerJoinedLobbyEvent(joinedID);
        }
    }
}

void SteamTransport::JoinLobby(CSteamID lobbyId)
{
    SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(lobbyId);
    m_LobbyEnterCallResult.Set(hSteamAPICall, this, &SteamTransport::OnLobbyEntered);
}

void SteamTransport::OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure)
{
    if (bIOFailure || pCallback->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
        return;

    currentLobbyId = CSteamID(pCallback->m_ulSteamIDLobby);

    if (OnLobbyJoinedEvent) {
        OnLobbyJoinedEvent();
    }
}

std::vector<CSteamID> SteamTransport::GetLobbyMembers()
{
    std::vector<CSteamID> playerIds;
    if (!currentLobbyId.IsValid()) return playerIds;

    int numMembers = SteamMatchmaking()->GetNumLobbyMembers(currentLobbyId);

    playerIds.reserve(numMembers);
    for (int i = 0; i < numMembers; i++)
    {
        uint64_t rawLobbyId = currentLobbyId.ConvertToUint64();
        uint64_t rawMemberId = SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex(SteamMatchmaking(), rawLobbyId, i);
        
        CSteamID id(rawMemberId); 
        playerIds.push_back(id);
    }
    
    return playerIds;
}

void SteamTransport::SendPacketToAll(Packet packet)
{
    for(CSteamID id : GetLobbyMembers())
    {
        if(id == localId)
            continue; 

        SteamNetworkingIdentity identity;
        identity.SetSteamID(id);

        SteamNetConnectionInfo_t info;
        SteamNetworkingMessages()->GetSessionConnectionInfo(identity, &info, nullptr);

        if (info.m_eState == k_ESteamNetworkingConnectionState_Connected)
        {
            SteamNetworkingMessages()->SendMessageToUser(identity, packet.data.data(), packet.data.size(), k_nSteamNetworkingSend_Reliable, 0);
        }
    }
}

void SteamTransport::SendUnreliablePacketToAll(Packet packet)
{
    for(CSteamID id : GetLobbyMembers())
    {
        if(id == localId)
            continue; 

        SteamNetworkingIdentity identity;
        identity.SetSteamID(id);

        SteamNetConnectionInfo_t info;
        SteamNetworkingMessages()->GetSessionConnectionInfo(identity, &info, nullptr);

        if (info.m_eState == k_ESteamNetworkingConnectionState_Connected)
        {
            SteamNetworkingMessages()->SendMessageToUser(identity, packet.data.data(), packet.data.size(), k_nSteamNetworkingSend_Unreliable, 0);
        }
    }
}

void SteamTransport::RecievePackets()
{
    ISteamNetworkingMessage* pIncomingMsg = nullptr;

    while (SteamNetworkingMessages()->ReceiveMessagesOnChannel(0, &pIncomingMsg, 1) > 0)
    {
        //Engine::network->ReceivePacket(static_cast<const uint8_t*>(pIncomingMsg->m_pData), pIncomingMsg->m_cbSize);
        pIncomingMsg->Release();
    }
}

void SteamTransport::Init()
{
    if (!SteamAPI_Init()) return;
    localId = SteamUser()->GetSteamID();
}

void SteamTransport::RunCallbacks() { SteamAPI_RunCallbacks(); }

// NEW: Gracefully close connections so Steam doesn't complain on exit
void SteamTransport::Shutdown() 
{ 
    if (currentLobbyId.IsValid())
    {
        for (CSteamID id : GetLobbyMembers())
        {
            if (id != localId)
            {
                SteamNetworkingIdentity identity;
                identity.SetSteamID(id);
                SteamNetworkingMessages()->CloseSessionWithUser(identity);
            }
        }
    }

    SteamAPI_Shutdown(); 
}