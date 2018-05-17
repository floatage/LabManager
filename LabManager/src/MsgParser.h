#ifndef MSGPARSER_H
#define MSGPARSER_H

#include "Common.h"

typedef std::function<void(JsonObjType&, ConnPtr)> MsgHandler;

typedef std::unordered_map<std::string, MsgHandler> MsgHandlerMap;

class MsgFamilyParser {
public:
	void registerFamilyHandler(const StringType& family, MsgHandler&&);
	void unRegisterFamilyHandler(const StringType& family);
	void familyParse(JsonObjType& msg, ConnPtr conn);
protected:
	MsgHandlerMap familyFuncMap;
};

class MsgActionParser {
public:
	void registerActionHandler(const StringType& action, MsgHandler&&);
	void unRegisterActionHandler(const StringType& action);
	void actionParse(JsonObjType& msg, ConnPtr conn);
protected:
	MsgHandlerMap actionFuncMap;
};

#endif // !MSGPARSER_H

