#define LOG_TAG "android.hardware.biometrics.fingerprint@2.3-service.oneplus_kona"
#define LOG_VERBOSE "android.hardware.biometrics.fingerprint@2.3-service.oneplus_kona"

#include <hardware/hardware.h>
#include <hardware/fingerprint.h>
#include "BiometricsFingerprint.h"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <fstream>

#define OP_ENABLE_FP_LONGPRESS 3
#define OP_DISABLE_FP_LONGPRESS 4
#define OP_RESUME_FP_ENROLL 8
#define OP_FINISH_FP_ENROLL 10

#define OP_DISPLAY_AOD_MODE 8
#define OP_DISPLAY_NOTIFY_PRESS 9
#define OP_DISPLAY_SET_DIM 10

#include <fstream>
#include <inttypes.h>
#include <unistd.h>

namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {
namespace V2_3 {
namespace implementation {

template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

BiometricsFingerprint::BiometricsFingerprint() {
    biometrics_2_1_service = IBiometricsFingerprint_2_1::getService();
    this->mFodCircleVisible = false;
    this->mVendorFpService = IVendorFingerprintExtensions::getService();
    this->mVendorDisplayService = IOneplusDisplay::getService();
}

Return<uint64_t> BiometricsFingerprint::setNotify(const sp<IBiometricsFingerprintClientCallback>& clientCallback) {
    return biometrics_2_1_service->setNotify(clientCallback);
}

Return<uint64_t> BiometricsFingerprint::preEnroll() {
    return biometrics_2_1_service->preEnroll();
}

Return<RequestStatus> BiometricsFingerprint::enroll(const hidl_array<uint8_t, 69>& hat, uint32_t gid, uint32_t timeoutSec) {
    this->mVendorFpService->updateStatus(OP_DISABLE_FP_LONGPRESS);
    this->mVendorFpService->updateStatus(OP_RESUME_FP_ENROLL);
    return biometrics_2_1_service->enroll(hat, gid, timeoutSec);
}

Return<RequestStatus> BiometricsFingerprint::postEnroll() {
    return biometrics_2_1_service->postEnroll();
}

Return<uint64_t> BiometricsFingerprint::getAuthenticatorId() {
    return biometrics_2_1_service->getAuthenticatorId();
}

Return<RequestStatus> BiometricsFingerprint::cancel() {
    this->mVendorFpService->updateStatus(OP_FINISH_FP_ENROLL);
    return biometrics_2_1_service->cancel();
}

Return<RequestStatus> BiometricsFingerprint::enumerate() {
    return biometrics_2_1_service->enumerate();
}

Return<RequestStatus> BiometricsFingerprint::remove(uint32_t gid, uint32_t fid) {
    return biometrics_2_1_service->remove(gid, fid);
}

Return<RequestStatus> BiometricsFingerprint::setActiveGroup(uint32_t gid, const hidl_string& storePath) {
    return biometrics_2_1_service->setActiveGroup(gid, storePath);
}

Return<RequestStatus> BiometricsFingerprint::authenticate(uint64_t operationId, uint32_t gid) {
    this->mVendorDisplayService->setMode(OP_DISPLAY_SET_DIM, 0);
    return biometrics_2_1_service->authenticate(operationId, gid);
}

Return<bool> BiometricsFingerprint::isUdfps(uint32_t) {
    return true;
}

Return<void> BiometricsFingerprint::onFingerDown(uint32_t, uint32_t, float, float) {
    this->mFodCircleVisible = true;
    this->mVendorDisplayService->setMode(OP_DISPLAY_SET_DIM, 1);
    this->mVendorDisplayService->setMode(OP_DISPLAY_NOTIFY_PRESS, 1);
    return Void();
}

Return<void> BiometricsFingerprint::onFingerUp() {
    this->mFodCircleVisible = false;
    this->mVendorDisplayService->setMode(OP_DISPLAY_SET_DIM, 0);
    this->mVendorDisplayService->setMode(OP_DISPLAY_NOTIFY_PRESS, 0);

    return Void();
}

}  // namespace implementation
}  // namespace V2_3
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
