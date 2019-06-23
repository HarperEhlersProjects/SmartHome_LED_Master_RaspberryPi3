#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

void vEventHandlerInit(void);

void vEventHandlerCheckEvents(void);

void vEventHandler(char uiVirtualSLA, char uiEventHandler);
void vEventHandler0(char uiVirtualSLA);
#endif /* EVENT_HANDLER_H */