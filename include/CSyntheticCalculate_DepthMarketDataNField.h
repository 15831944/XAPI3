#pragma once

#include "synthetic/CSyntheticMarketData.h"

class CSyntheticCalculateFactory_XAPI:public CSyntheticCalculateFactory
{
public:
	virtual CSyntheticCalculate* Create(string method);
};


class CSyntheticCalculate_DepthMarketDataNField_OpenInterest :public CSyntheticCalculate
{
public:
	virtual void Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
	virtual void Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
};

class CSyntheticCalculate_DepthMarketDataNField_Weight :public CSyntheticCalculate
{
public:
	virtual void Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
	virtual void Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
};

class CSyntheticCalculate_DepthMarketDataNField_Copy :public CSyntheticCalculate
{
public:
	virtual void Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
	virtual void Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
};

class CSyntheticCalculate_DepthMarketDataNField_Spread :public CSyntheticCalculate
{
public:
	virtual void Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
	virtual void Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut);
	virtual void End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut);
};

void CSyntheticCalculate_DepthMarketDataNField_OpenInterest::Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	memset(pO, 0, sizeof(DepthMarketDataNField));
	memset(pOut, 0, sizeof(LegData));

	pOut->pBuf = (char*)pO;
}

void CSyntheticCalculate_DepthMarketDataNField_OpenInterest::Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	pO->OpenInterest += pI->OpenInterest;
	pO->Volume += pI->Volume;
	// ����������������һ��sum
	pOut->sum += pI->OpenInterest;
}

void CSyntheticCalculate_DepthMarketDataNField_OpenInterest::ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);
	// ����Ȩ�أ�����ָ��Ȩ�ص����ݲ���Ҫ�޸ģ���ָ���ĳɷݹ�
	pIn->weight = pI->OpenInterest / pOut->sum;
	// �����¼۸�
	pO->LastPrice += pI->LastPrice * pIn->weight;
}

void CSyntheticCalculate_DepthMarketDataNField_OpenInterest::End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	pO->Size = sizeof(DepthMarketDataNField);
	pO->BidCount = 0;
	pO->ActionDay = pI->ActionDay;
	pO->TradingDay = pI->TradingDay;
	pO->UpdateTime = pI->UpdateTime;
	pO->UpdateMillisec = pI->UpdateMillisec;
	strcpy(pO->ExchangeID, pI->ExchangeID);

	strcpy(pO->InstrumentID, pCall->GetProduct().c_str());
	strcpy(pO->Symbol, pO->InstrumentID);
}

void CSyntheticCalculate_DepthMarketDataNField_Weight::Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	memset(pO, 0, sizeof(DepthMarketDataNField));
	memset(pOut, 0, sizeof(LegData));

	pOut->pBuf = (char*)pO;
}

void CSyntheticCalculate_DepthMarketDataNField_Weight::Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	// ������Ӧ��Ȩ�غ�Ϊ1��100
	// Ŀǰû���õ�
	// pOut->weight += pIn->weight;
}

void CSyntheticCalculate_DepthMarketDataNField_Weight::ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);
	// �����¼۸������Ȩ���ڼ���ʱ��������
	pO->LastPrice += pI->LastPrice * pIn->weight;
}

void CSyntheticCalculate_DepthMarketDataNField_Weight::End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	pO->Size = sizeof(DepthMarketDataNField);
	pO->BidCount = 0;
	pO->ActionDay = pI->ActionDay;
	pO->TradingDay = pI->TradingDay;
	pO->UpdateTime = pI->UpdateTime;
	pO->UpdateMillisec = pI->UpdateMillisec;
	strcpy(pO->ExchangeID, pI->ExchangeID);

	strcpy(pO->InstrumentID, pCall->GetProduct().c_str());
	strcpy(pO->Symbol, pO->InstrumentID);
}

void CSyntheticCalculate_DepthMarketDataNField_Copy::Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	//memset(pO, 0, sizeof(DepthMarketDataNField));
	// ��������
	memcpy(pO, pI, pI->Size);
	memset(pOut, 0, sizeof(LegData));

	pOut->pBuf = (char*)pO;
}

void CSyntheticCalculate_DepthMarketDataNField_Copy::Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);
}

void CSyntheticCalculate_DepthMarketDataNField_Copy::ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);
}

void CSyntheticCalculate_DepthMarketDataNField_Copy::End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	strcpy(pO->InstrumentID, pCall->GetProduct().c_str());
	strcpy(pO->Symbol, pO->InstrumentID);
}

void CSyntheticCalculate_DepthMarketDataNField_Spread::Begin(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	memset(pO, 0, sizeof(DepthMarketDataNField));
	memset(pOut, 0, sizeof(LegData));

	pOut->pBuf = (char*)pO;

	pOut->askSize = DBL_MAX;
	pOut->bidSize = DBL_MAX;
}

void CSyntheticCalculate_DepthMarketDataNField_Spread::Sum(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);
}

void CSyntheticCalculate_DepthMarketDataNField_Spread::ForEach(CSyntheticMarketData* pCall, LegData* pIn, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pIn->pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	pO->LastPrice += pI->LastPrice * pIn->weight;

	auto bid = GetBid(pI, 1);
	auto ask = GetAsk(pI, 1);

	if (pIn->weight > 0)
	{
		if (ask == nullptr)
		{
			pOut->askSize = 0;
			pOut->ask = 0;
		}
		else
		{
			// pOut->askSize = min(pOut->askSize, ask->Size);
			pOut->askSize = pOut->askSize < ask->Size ? pOut->askSize : ask->Size;
			pOut->ask += ask->Price * pIn->weight;
		}

		if (bid == nullptr)
		{
			pOut->bidSize = 0;
			pOut->bid = 0;
		}
		else
		{
			// pOut->bidSize = min(pOut->bidSize, bid->Size);
			pOut->bidSize = pOut->bidSize < bid->Size ? pOut->bidSize : bid->Size;
			pOut->bid += bid->Price * pIn->weight;
		}
	}
	else
	{
		if (ask == nullptr)
		{
			pOut->bidSize = 0;
			pOut->bid = 0;
		}
		else
		{
			// pOut->bidSize = min(pOut->bidSize, ask->Size);
			pOut->bidSize = pOut->bidSize < ask->Size ? pOut->bidSize : ask->Size;
			pOut->bid += ask->Price * pIn->weight;
		}

		if (bid == nullptr)
		{
			pOut->askSize = 0;
			pOut->ask = 0;
		}
		else
		{
			// pOut->askSize = min(pOut->askSize, bid->Size);
			pOut->askSize = pOut->askSize < bid->Size ? pOut->askSize : bid->Size;
			pOut->ask += bid->Price * pIn->weight;
		}
	}
}

void CSyntheticCalculate_DepthMarketDataNField_Spread::End(CSyntheticMarketData* pCall, char* pBuf, LegData* pOut)
{
	auto pI = (DepthMarketDataNField*)(pBuf);
	auto pO = (DepthMarketDataNField*)(pOut->pBuf);

	pO->Size = sizeof(DepthMarketDataNField);
	pO->BidCount = 0;
	pO->ActionDay = pI->ActionDay;
	pO->TradingDay = pI->TradingDay;
	pO->UpdateTime = pI->UpdateTime;
	pO->UpdateMillisec = pI->UpdateMillisec;
	strcpy(pO->ExchangeID, pI->ExchangeID);

	strcpy(pO->InstrumentID, pCall->GetProduct().c_str());
	strcpy(pO->Symbol, pO->InstrumentID);


	InitBidAsk(pO);

	do
	{
		if (pOut->bidSize == 0)
			break;
		AddBid(pO, pOut->bid, pOut->bidSize, 0);
	} while (false);

	do
	{
		if (pOut->askSize == 0)
			break;
		AddAsk(pO, pOut->ask, pOut->askSize, 0);

	} while (false);
}

CSyntheticCalculate* CSyntheticCalculateFactory_XAPI::Create(string method)
{
	if (method == "OpenInterest")
	{
		return new CSyntheticCalculate_DepthMarketDataNField_OpenInterest();
	}
	else if (method == "Weight")
	{
		return new CSyntheticCalculate_DepthMarketDataNField_Weight();
	}
	else if (method == "Copy")
	{
		return new CSyntheticCalculate_DepthMarketDataNField_Copy();
	}
	else if (method == "Spread")
	{
		return new CSyntheticCalculate_DepthMarketDataNField_Spread();
	}
	return nullptr;
}

