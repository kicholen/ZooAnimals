#include "GooglePlayInAppPurchaseManager.h"

GooglePlayInAppPurchaseManager GooglePlayInAppPurchaseManager::instance;

GooglePlayInAppPurchaseManager::GooglePlayInAppPurchaseManager() {
	_isGooglePlayInAppPurchaseAvailable = S3E_FALSE;
	publicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhYVwzJU8hGzkDTUpuUUXuSwWHdM6nlqC2WEjd+mE7qEIJn98XEBJurW8zMtnzJtAINXuhMEAKWBM3u6nTe06pxSruLs0HrKzaK/uR2c5nDJZWq8kprcavOC7knqWWiVmpC1OFbIZ452wH8CXNpQM71b0m+RH2VP08ltbN++iQMHSCES4YPLEV2ywvW6BnGqgVDZRXXDnDFj/9vHdZeWeQKlhtUa4sDxlKUh/D2M9BN2/zCiAPeesWpIlkuPKU3A04nIBT6ee+GPk8Pflz5ujDiV3VclubERuyV8u2pPWXYdbdZdeq2id4FhFniJDI4FPZPgx5TSMuhztEHoxaeiZlQIDAQAB";
}

GooglePlayInAppPurchaseManager::~GooglePlayInAppPurchaseManager() {

}

void GooglePlayInAppPurchaseManager::init() {
	_isGooglePlayInAppPurchaseAvailable = s3eAndroidGooglePlayBillingAvailable();
	if (_isGooglePlayInAppPurchaseAvailable) {
		s3eAndroidGooglePlayBillingStart(publicKey);
		registerListeners();
		_isGooglePlayInAppPurchaseAvailable = s3eAndroidGooglePlayBillingIsSupported();
	}
}

void GooglePlayInAppPurchaseManager::queryShop() {
	if (_isGooglePlayInAppPurchaseAvailable) {
		s3eDebugOutputString("Marmalade-GooglePlayIAP- query shop");
		const char *inAppSkus[] = {
			"test_product"//"levels_ads_unblock"
		};
		s3eAndroidGooglePlayBillingRequestProductInformation(inAppSkus, sizeof(inAppSkus) / sizeof(const char*), NULL, NULL);
	}
}

void GooglePlayInAppPurchaseManager::restorePurchases() {
	if (_isGooglePlayInAppPurchaseAvailable) {
		s3eDebugOutputString("Marmalade-GooglePlayIAP- restore purchases");
		s3eAndroidGooglePlayBillingRestoreTransactions();
	}
}

void GooglePlayInAppPurchaseManager::purchaseProduct(const char* productId, bool isConsumable) {
	if (_isGooglePlayInAppPurchaseAvailable) {
		s3eDebugOutputString("Marmalade-GooglePlayIAP- purchase product");
		//string randomPayload = CMath::stringFormat("TestPayload %d", int(FlashUtils::CMath::Rand(1, 10000)));
		s3eAndroidGooglePlayBillingRequestPurchase(productId, isConsumable);//, randomPayload.c_str());
	}
}
void GooglePlayInAppPurchaseManager::consumeProduct() {
	if (_isGooglePlayInAppPurchaseAvailable) {
		s3eDebugOutputString("Marmalade-GooglePlayIAP- consume product");
		s3eAndroidGooglePlayBillingConsumeItem("dupa");
	}
}

