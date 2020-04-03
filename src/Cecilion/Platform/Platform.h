#pragma once

// TODO Dynamically evaluate what platform we're on.
// Included by Window.h. Maybe not the most optimal way of doing it.
// The user should be able to define default platform and render context and
// be able to switch out the context if they want
#define CECILION_PLATFORM_WINDOWS
#define RENDER_CONTEXT_GL