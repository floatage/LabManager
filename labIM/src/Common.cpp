#include "Common.h"

#include <windows.h>
#include <iphlpapi.h>

#pragma comment(lib,"iphlpapi.lib")

ConnImplType ConnType::CONN_TEMP = 0;
ConnImplType ConnType::CONN_CHILD = 1;
ConnImplType ConnType::CONN_BROTHER = 2;
ConnImplType ConnType::CONN_PARENT = 3;

ConnImplType ConnType::getConnType(HostRole source, HostRole dest) {
	static ConnImplType typeMap[5][5][1]{-1};
	static bool isInit = false;

	if (!isInit) {
		//ROLE_NULL, ROLE_MASTER, ROLE_ROUTER, ROLE_MEMBER, ROLE_ADMIN
		typeMap[0][0][0] = 0; typeMap[0][1][0] = 0; typeMap[0][2][0] = 0; typeMap[0][3][0] = 0; typeMap[0][4][0] = 0;
		typeMap[1][0][0] = 0; typeMap[1][1][0] = 2; typeMap[1][2][0] = 1; typeMap[1][3][0] = 0; typeMap[1][4][0] = 0;
		typeMap[2][0][0] = 0; typeMap[2][1][0] = 3; typeMap[2][2][0] = 2; typeMap[2][3][0] = 1; typeMap[2][4][0] = 0;
		typeMap[3][0][0] = 0; typeMap[3][1][0] = 0; typeMap[3][2][0] = 3; typeMap[3][3][0] = 2; typeMap[3][4][0] = 0;
		typeMap[4][0][0] = 0; typeMap[4][1][0] = 0; typeMap[4][2][0] = 0; typeMap[4][3][0] = 0; typeMap[4][4][0] = 0;
		
		isInit = true;
	}

	return typeMap[source][dest][0];
}

ushort HostDescription::udpPort = 8888;
ushort HostDescription::tcpPort = 8889;

void HostDescription::setTcpPort(ushort newPort) {

}

void HostDescription::setUdpPort(ushort newPort) {

}

ulonglong getAvailMemory() {
	MEMORYSTATUS ms;
	::GlobalMemoryStatus(&ms);
	return (ulonglong)ms.dwAvailPhys + (ulonglong)ms.dwAvailVirtual;
}

uint getCpuFrequency() {
	DWORD low1 = 0, high1 = 0, low2 = 0, high2 = 0;
	LARGE_INTEGER fq, st, ed;

	::QueryPerformanceFrequency(&fq);
	::QueryPerformanceCounter(&st);

	__asm {
		rdtsc
		mov low1, eax
		mov high1, edx
	}

	::Sleep(15);
	::QueryPerformanceCounter(&ed);

	__asm {
		rdtsc
		mov low2, eax
		mov high2, edx
	}

	LONGLONG begin = (LONGLONG)high1 << 32 | low1;
	LONGLONG end = (LONGLONG)high2 << 32 | low2;

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	return uint(((end - begin)*fq.QuadPart / (ed.QuadPart - st.QuadPart) * si.dwNumberOfProcessors) >> 15);
}

StringType getLocalIp()
{
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), boost::asio::ip::host_name(), "");
	udp::resolver::iterator iter = resolver.resolve(query);
	udp::resolver::iterator end;
	while (iter != end) {
		udp::endpoint ep = *iter;
		return StringType(ep.address().to_string().c_str());
	}

	return StringType();
}

ulong getIpUlong(const StringType& ip){
	struct in_addr in;
	inet_pton(AF_INET, ip.c_str(), (void *)&in);
	return in.S_un.S_addr;
}

StringType getMac(const StringType& ip)
{
	BYTE mac[6];
	ULONG macLen = 6;
	char macAddrDest[18];
	memset(mac, 0xff, sizeof(mac));

	DWORD hr = SendARP(getIpUlong(ip), 0, mac, &macLen);
	if (hr == 0)
	{
		PBYTE  pbMac = (PBYTE)mac;
		for (ULONG i = 0; i < macLen; i++)
			mac[i] = pbMac[i];

		sprintf_s(macAddrDest, "%02X-%02X-%02X-%02X-%02X-%02X",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		return macAddrDest;
	}

	return StringType();
}

StringType getMac()
{
	return getMac(getLocalIp());
}

void setHostArp(const StringType& ip, const StringType& mac) {
	
}
