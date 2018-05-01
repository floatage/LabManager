#include "MsgParser.h"

void MsgFamilyParser::registerFamilyHandler(const StringType & family, MsgHandler&& handler)
{
	familyFuncMap[family] = handler;
}

void MsgFamilyParser::unRegisterFamilyHandler(const StringType& family)
{
	familyFuncMap.erase(family);
}

void MsgFamilyParser::familyParse(JsonObjType & msg, ConnPtr conn)
{
	if (msg.contains("family"))
	{
		auto family = msg["family"].toString().toStdString();
		msg.remove("family");
		if (familyFuncMap.find(family) != familyFuncMap.end())
		{
			familyFuncMap[family](msg, conn);
		}
	}
}

void MsgActionParser::registerActionHandler(const StringType & action, MsgHandler &&handler)
{
	actionFuncMap[action] = handler;
}

void MsgActionParser::unRegisterActionHandler(const StringType & action)
{
	actionFuncMap.erase(action);
}

void MsgActionParser::actionParse(JsonObjType& msg, ConnPtr conn)
{
	if (msg.contains("action")) {
		auto action = msg["action"].toString().toStdString();
		if (actionFuncMap.find(action) != actionFuncMap.end()) {
			msg.remove("action");
			actionFuncMap[action](msg, conn);
		}
	}
}
