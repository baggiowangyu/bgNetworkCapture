#ifndef _BG_NETWORK_ADAPTER_H_
#define _BG_NETWORK_ADAPTER_H_

#include <string>

class bgNetworkAdapters
{
public:
	//   ≈‰∆˜–≈œ¢
	typedef struct _adapter_
	{
		std::string name_;
		std::string descripte_;

	} bg_adapter, *bg_adapter_ptr;

public:
	int OnInit();
	void OnClose();

public:

};

#endif//_BG_NETWORK_ADAPTER_H_