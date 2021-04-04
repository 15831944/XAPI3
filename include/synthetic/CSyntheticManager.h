#pragma once

#include <map>
#include <set>
#include <list>
#include <mutex>

#include "CSyntheticMarketData.h"

class CSyntheticManager
{
public:
	~CSyntheticManager()
	{
		for (auto iter = m_map_by_product.begin(); iter != m_map_by_product.end(); iter++)
		{
			delete iter->second;
		}
		m_map_by_product.clear();
		for (auto iter = m_map_by_instrument.begin(); iter != m_map_by_instrument.end(); iter++)
		{
			delete[] iter->second;
		}
		m_map_by_instrument.clear();
	}

	// ����ԭ�������Upate��IF1802ͬʱ����IF000��IF888�����У����Ծ�����ǰ����
	CSyntheticMarketData* Register(const char* product, set<string> emits, map<string, double> legs, CSyntheticCalculate* pCal)
	{
		// ����ע����ٸ��ϳɺ�Լ
		CSyntheticMarketData* pSMD = nullptr;
		auto it = m_map_by_product.find(product);
		if (it == m_map_by_product.end())
		{
			pSMD = new CSyntheticMarketData(product, emits, pCal);
			m_map_by_product.insert(pair<string, CSyntheticMarketData*>(product, pSMD));
		}
		else
		{
			pSMD = it->second;
		}

		// �Ǽ���Ҫ�õ�����ʵ�ʺ�Լ
		for (auto iter = legs.begin(); iter != legs.end(); iter++)
		{
			char* pBuf = nullptr;
			auto it = m_map_by_instrument.find(iter->first);
			if (it == m_map_by_instrument.end())
			{
				// ����
				pBuf = new char[LEG_DATA_BUF_LEN]();
				memset(pBuf, 0, LEG_DATA_BUF_LEN);
				m_map_by_instrument.insert(pair<string, char*>(iter->first, pBuf));
			}
			else
			{
				pBuf = it->second;
			}

			pSMD->Create(iter->first.c_str(), iter->second, pBuf);

			// ����ʵ�̺�Լ�����ܺϳɵĺ�Լ 
			auto it2 = m_map_set_by_instrument.find(iter->first);
			if (it2 == m_map_set_by_instrument.end())
			{
				auto p = set<CSyntheticMarketData*>();
				p.insert(pSMD);
				m_map_set_by_instrument.insert(pair<string, set<CSyntheticMarketData*>>(iter->first, p));
			}
			else
			{
				it2->second.insert(pSMD);
			}
		}
		// ������û���õ�Ȩ�أ�����һ��
		//pSMD->UpdateWeight();
		return pSMD;
	}
	set<string> UnRegister(const char* product)
	{
		auto it = m_map_by_product.find(product);
		if (it == m_map_by_product.end())
		{
			// û�ҵ������ǿ���Ц�İ�
			set<string> ss;
			return ss;
		}
		else
		{
			// �ҵ��ϳ���ָ��
			auto p = it->second;
			// �ҵ��ˣ���Ҫ�õ��õ�����Щ��Լ
			auto ss = p->GetInstruments();
			for (auto s1 = ss.begin(); s1 != ss.end(); s1++)
			{
				auto s2 = m_map_set_by_instrument.find(s1->data());
				if (s2 == m_map_set_by_instrument.end())
				{
					// û�ҵ������ǿ���Ц�İ�
				}
				else
				{
					// ����ʵ�ʺ�Լ������ϳɵĺϳ���ɾ��
					s2->second.erase(p);
					if (s2->second.size() == 0)
					{
						// û��Ҫ���ģ���Ҫɾ��
						m_map_by_instrument.erase(s1->data());
					}
				}
			}

			m_map_by_product.erase(product);

			return ss;
		}
	}

	// �ȸ��ݺ�Լ�����º�Լ�ڴ���
	// ע�⣬���ص��ڴ��0��λ�Ǳ��λ
	char* Update(const char* instrument, void* pData, int size)
	{
		auto it = m_map_by_instrument.find(instrument);
		if (it == m_map_by_instrument.end())
		{
			// û�ҵ�
			return nullptr;
		}
		else
		{
			//// �õ�һ���ֽڱ�������Ѿ��յ�
			//*it->second = 1;
			//memcpy(it->second+1, pData, size);
			memcpy(it->second, pData, size);
			return it->second;
		}
	}
	
	set<CSyntheticMarketData*> GetByInstrument(const char* instrument)
	{
		auto it2 = m_map_set_by_instrument.find(instrument);
		if (it2 == m_map_set_by_instrument.end())
		{
			auto p = set<CSyntheticMarketData*>();
			return p;
		}
		else
		{
			return it2->second;
		}
	}

private:
	// ��¼�Ķ��ٸ��ϳɺ�Լ�������
	map<string, CSyntheticMarketData*>	m_map_by_product;
	// ֻ��¼��ԭʼ������buf��ÿ���������ж��ָ��ָ����
	map<string, char*>	m_map_by_instrument;
	// ���������ֻ�����������Ӧ������
	map<string, set<CSyntheticMarketData*>>	m_map_set_by_instrument;

	mutex							m_cs;
};


