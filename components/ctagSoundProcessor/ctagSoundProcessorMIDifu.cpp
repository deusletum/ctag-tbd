#include "ctagSoundProcessorMIDifu.hpp"
#include <iostream>
#include "helpers/ctagFastMath.hpp"

using namespace CTAG::SP;

ctagSoundProcessorMIDifu::ctagSoundProcessorMIDifu() {
    setIsStereo();
    model = std::make_unique<ctagSPDataModel>(id, isStereo);
    model->LoadPreset(0);

    fx_buffer = (float *) heap_caps_malloc(8192 * sizeof(float),
                                               MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (fx_buffer == NULL) {
        ESP_LOGE("MIDifu", "Could not allocate shared buffer!");
    }
    fx.Init(fx_buffer);
    fx.Clear();
}

void ctagSoundProcessorMIDifu::Process(const ProcessData &data) {
    float fTime = time / 4095.f;
    float fAmount = amount / 4095.f;

    if(cv_time != -1){
        fTime = HELPERS::fastfabs(data.cv[cv_time]);
    }
    if(cv_amount != -1){
        fAmount = HELPERS::fastfabs(data.cv[cv_amount]);
    }

    float io[bufSz];
    for(int i=0;i<bufSz;i++){
        io[i] = data.buf[i*2 + processCh];
    }
    fx.Process(fAmount, fTime, io, bufSz);
    for(int i=0;i<bufSz;i++){
        data.buf[i*2 + processCh] =  io[i];
    }
}

ctagSoundProcessorMIDifu::~ctagSoundProcessorMIDifu() {
    heap_caps_free(fx_buffer);
}

const char *ctagSoundProcessorMIDifu::GetCStrID() const {
    return id.c_str();
}


void ctagSoundProcessorMIDifu::setParamValueInternal(const string& id, const string& key, const int val) {
// autogenerated code here
// sectionCpp0
if(id.compare("amount") == 0){
	if(key.compare("current") == 0){
		amount = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_amount = val;
	}
	return;
}
if(id.compare("time") == 0){
	if(key.compare("current") == 0){
		time = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_time = val;
	}
	return;
}
// sectionCpp0


}

void ctagSoundProcessorMIDifu::loadPresetInternal() {
// autogenerated code here
// sectionCpp1
amount = model->GetParamValue("amount", "current");
cv_amount = model->GetParamValue("amount", "cv");
time = model->GetParamValue("time", "current");
cv_time = model->GetParamValue("time", "cv");
// sectionCpp1


}
