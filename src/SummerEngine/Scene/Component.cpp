#include "Component.h"
#include "Object.h"

Component::Component() {}
Component::~Component() {}

void Component::Start() {}

void Component::Update(float delta) {}
void Component::SteppedUpdate(float delta) {}

void Component::OnDestroy() {}