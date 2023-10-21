# Broadcast bus / Event system

The broadcasting system uses a store-and-reference technique, which means that all messages are stored internally 
until all active actors have responded. Messages themselves are forwarded as a reference whenever a actors wants 
to fetch the information, and when all actors have responded the message will be deleted from the system.

An advantage of not executing messages immediately is that
sending a message will not stop the current thread/operation. However, memory leaks can occur if an actor does not 
respond to its event. Another downside is that an important message might not be taken care of until the next frame, 
or even later.

## Event actor
Event actors contain one or more inboxes for events and may, at any time, check the inbox for new messages. 
Before a message can be acted upon, a callback function must be added to the desired inbox.

It is currently not possible to skip messages or temporarily unsubscribe. This means that multiple messages of the same 
type will be received, even if only the last instance is important.