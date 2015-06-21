#ifndef _GOOGLEPLAYINAPPPURCHASEMANAGER_
#define _GOOGLEPLAYINAPPPURCHASEMANAGER_

//#include "s3eAndroidGooglePlayBilling.h"
#include "s3e.h"
#include "FlashUtils.h"
#include "Settings.h"

using namespace FlashUtils;

class GooglePlayInAppPurchaseManager
{
public:
	static GooglePlayInAppPurchaseManager instance;

	GooglePlayInAppPurchaseManager();
	~GooglePlayInAppPurchaseManager();

	void init();

	void queryShop();
	void restorePurchases();
	void purchaseProduct(const char* productId, bool isSubscription);
	void consumeProduct();

protected:
	void registerListeners() {
		/*s3eAndroidGooglePlayBillingRegister(S3E_ANDROIDGOOGLEPLAYBILLING_LIST_PRODUCTS_CALLBACK, listCallback, NULL);
		s3eAndroidGooglePlayBillingRegister(S3E_ANDROIDGOOGLEPLAYBILLING_RESTORE_CALLBACK, restoreCallback, NULL);
		s3eAndroidGooglePlayBillingRegister(S3E_ANDROIDGOOGLEPLAYBILLING_PURCHASE_CALLBACK, purchaseCallback, NULL);
		s3eAndroidGooglePlayBillingRegister(S3E_ANDROIDGOOGLEPLAYBILLING_CONSUME_CALLBACK, consumeCallback, NULL);*/
	}
	void unregisterListeners() {
		/*s3eAndroidGooglePlayBillingUnRegister(S3E_ANDROIDGOOGLEPLAYBILLING_LIST_PRODUCTS_CALLBACK, listCallback);
		s3eAndroidGooglePlayBillingUnRegister(S3E_ANDROIDGOOGLEPLAYBILLING_RESTORE_CALLBACK, restoreCallback);
		s3eAndroidGooglePlayBillingUnRegister(S3E_ANDROIDGOOGLEPLAYBILLING_PURCHASE_CALLBACK, purchaseCallback);
		s3eAndroidGooglePlayBillingUnRegister(S3E_ANDROIDGOOGLEPLAYBILLING_CONSUME_CALLBACK, consumeCallback);*/
	}
private:
	static int32 listCallback(void *systemData, void *userData) {
		/*if (systemData) {
			s3eAndroidGooglePlayBillingSkuResponse *skus = (s3eAndroidGooglePlayBillingSkuResponse*)systemData;
			string str;

			if (skus->m_ErrorMsg) {
				str = FlashUtils::CMath::stringFormat("List Sku returned : %d, %s", (int)skus->m_Status, skus->m_ErrorMsg);
				s3eDebugOutputString(str.c_str());
			}
			if (skus->m_Status == S3E_ANDROIDGOOGLEPLAYBILLING_RESULT_OK) {
				str = FlashUtils::CMath::stringFormat("%d items returned", skus->m_NumProducts);
				s3eDebugOutputString(str.c_str());

				for (int i = 0; i < skus->m_NumProducts; i++) {
					s3eDebugOutputString("{");
					s3eAndroidGooglePlayBillingItemInfo *item = &skus->m_Products[i];
					s3eDebugOutputString(CMath::stringFormat(" m_ProductID     : %s", item->m_ProductID).c_str());
					s3eDebugOutputString(CMath::stringFormat(" m_Type          : %s", item->m_Type).c_str());
					s3eDebugOutputString(CMath::stringFormat(" m_Price         : %s", item->m_Price).c_str());
					s3eDebugOutputString(CMath::stringFormat(" m_Title         : %s", item->m_Title).c_str());
					s3eDebugOutputString(CMath::stringFormat(" m_Description   : %s", item->m_Description).c_str());
					s3eDebugOutputString("}");
				}
			}
		}*/
		return 1;
	}

	static int32 restoreCallback(void* systemData, void* userData) {
		/*if (systemData) {
			bool wasProductPurchased = false;
			s3eAndroidGooglePlayBillingRestoreResponse *rr = (s3eAndroidGooglePlayBillingRestoreResponse*)systemData;
			string str;

			bool wasErrorCatched = false;

			if (rr->m_ErrorMsg) {
				str = FlashUtils::CMath::stringFormat("Restore returned : %d, %s", (int)rr->m_Status, rr->m_ErrorMsg);
				s3eDebugOutputString(str.c_str());
				wasErrorCatched = true;
			}
			if (rr->m_Status == S3E_ANDROIDGOOGLEPLAYBILLING_RESULT_OK) {
				str = FlashUtils::CMath::stringFormat("%d items returned", rr->m_NumPurchases);
				s3eDebugOutputString(str.c_str());

				if (rr->m_NumPurchases > 0) {
					for (int i = 0; i < rr->m_NumPurchases; i++) {
						s3eDebugOutputString("{");
						s3eAndroidGooglePlayBillingPurchase *item = &rr->m_Purchases[i];
						s3eDebugOutputString(CMath::stringFormat(" m_OrderID           : %s",item->m_OrderID).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_PackageID         : %s",item->m_PackageID).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_ProductId         : %s",item->m_ProductId).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_PurchaseTime      : %d",item->m_PurchaseTime).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_PurchaseState     : %d",item->m_PurchaseState).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_PurchaseToken     : %s",item->m_PurchaseToken).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_DeveloperPayload  : %s",item->m_DeveloperPayload).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_JSON              : %s",item->m_JSON).c_str());
						s3eDebugOutputString(CMath::stringFormat(" m_Signature         : %s",item->m_Signature).c_str());
						s3eDebugOutputString("}");

						if (string(item->m_ProductId) == "test_product" && item->m_PurchaseState == 0) {
							wasProductPurchased = true;
						}
					}
				}
			}
			if (wasProductPurchased) {
				Settings::instance.getValue("level_4").set_value(1);
				Settings::instance.getValue("level_5").set_value(1);
				Settings::instance.getValue("level_6").set_value(1);
				Settings::instance.getValue("unblock_ads").set_value(1);
				Settings::instance.save();
			}
			else {
				Settings::instance.getValue("level_5").set_value(0);
				Settings::instance.getValue("level_6").set_value(0);
				Settings::instance.getValue("unblock_ads").set_value(0);
				Settings::instance.save();
			}
		}*/
		return 1;
	}

	static int32 purchaseCallback(void* systemData, void* userData) {
		/*if (systemData) {
			bool wasProductPurchased = false;
			s3eAndroidGooglePlayBillingPurchaseResponse *pr = (s3eAndroidGooglePlayBillingPurchaseResponse*)systemData;
			string str;

			if (pr->m_ErrorMsg) {
				str = FlashUtils::CMath::stringFormat("Purchase returned : %d, %s", (int)pr->m_Status, pr->m_ErrorMsg);
				s3eDebugOutputString(str.c_str());
			}
			if (pr->m_Status == S3E_ANDROIDGOOGLEPLAYBILLING_RESULT_OK) {
				s3eAndroidGooglePlayBillingPurchase *item = pr->m_PurchaseDetails;
				s3eDebugOutputString(CMath::stringFormat(" m_OrderID           : %s",item->m_OrderID).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_PackageID         : %s",item->m_PackageID).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_ProductId         : %s",item->m_ProductId).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_PurchaseTime      : %d",item->m_PurchaseTime).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_PurchaseState     : %d",item->m_PurchaseState).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_PurchaseToken     : %s",item->m_PurchaseToken).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_DeveloperPayload  : %s",item->m_DeveloperPayload).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_JSON              : %s",item->m_JSON).c_str());
				s3eDebugOutputString(CMath::stringFormat(" m_Signature         : %s",item->m_Signature).c_str());


				if (string(item->m_ProductId) == "test_product" && item->m_PurchaseState == 0) {
					wasProductPurchased = true;
				}
			}
							
			if (wasProductPurchased) {
				Settings::instance.getValue("level_4").set_value(1);
				Settings::instance.getValue("level_5").set_value(1);
				Settings::instance.getValue("level_6").set_value(1);
				Settings::instance.getValue("unblock_ads").set_value(1);
				Settings::instance.save();
			}
		}*/
		return 1;
	}

	static int32 consumeCallback(void* systemData, void* userData) {
		/*if (systemData) {
			s3eAndroidGooglePlayBillingConsumeResponse *cr = (s3eAndroidGooglePlayBillingConsumeResponse*)systemData;
			string str;

			if (cr->m_ErrorMsg) {
				str = CMath::stringFormat("Purchase returned : %d, %s", (int)cr->m_Status, cr->m_ErrorMsg);
				s3eDebugOutputString(str.c_str());
			}
			if (cr->m_Status == S3E_ANDROIDGOOGLEPLAYBILLING_RESULT_OK)
				s3eDebugOutputString("consumed");
		}*/
		return 1;
	}
private:
	s3eBool _isGooglePlayInAppPurchaseAvailable;
	const char *publicKey;
};

#endif