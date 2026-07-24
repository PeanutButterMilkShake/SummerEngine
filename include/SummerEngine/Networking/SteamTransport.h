#pragma once

#include <steam/steam_api.h>
#include <vector>
#include <functional>
#include "Engine.h"
#include "ITransport.h"

class SteamTransport : public ITransport
{
public:
    static CSteamID currentLobbyId;
    static CSteamID localId;
    static uint32_t localCounter;
    int maxMembers;

    std::function<void()> OnLobbyJoinedEvent;
    std::function<void(CSteamID)> OnPlayerJoinedLobbyEvent;

    // Lobby functions
    void HostLobby(int _maxMembers = 4); 
    void JoinLobby(CSteamID lobbyId);
    void StartInviteOverlay();
    std::vector<CSteamID> GetLobbyMembers();

    // Original Networking functions
    void SendPacketToAll(Packet packet);
    void SendUnreliablePacketToAll(Packet packet);
    void RecievePackets();

    // ITransport interface implementation
    void Init() override;
    void RunCallbacks() override;
    void Shutdown() override;
    void SendToAll(Packet packet, bool reliable) override;
    void ReceiveAll() override;

private:
    // Callback triggered when host creates lobby
    CCallResult<SteamTransport, LobbyCreated_t> m_LobbyCreatedCallResult;
    void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);

    // Callback triggered when host/client enters a lobby
    CCallResult<SteamTransport, LobbyEnter_t> m_LobbyEnterCallResult;
    void OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure);

    STEAM_CALLBACK(SteamTransport, OnGameLobbyJoinRequested, GameLobbyJoinRequested_t);
    
    // Accept incoming peer-to-peer connection requests
    STEAM_CALLBACK(SteamTransport, OnSessionRequest, SteamNetworkingMessagesSessionRequest_t);

    // NEW: Listens for user state changes (joining/leaving)
    STEAM_CALLBACK(SteamTransport, OnLobbyChatUpdate, LobbyChatUpdate_t);
};