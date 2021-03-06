#include "ctagSoundProcessorMIVerb.hpp"
#include <iostream>
#include "esp_heap_caps.h"
#include "helpers/ctagFastMath.hpp"

using namespace CTAG::SP;

ctagSoundProcessorMIVerb::ctagSoundProcessorMIVerb() {
    setIsStereo();
    model = std::make_unique<ctagSPDataModel>(id, isStereo);
    model->LoadPreset(0);

    reverb_buffer = (float *) heap_caps_malloc(32768 * sizeof(float),
                                                  MALLOC_CAP_SPIRAM);//MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (reverb_buffer == NULL) {
        ESP_LOGE("MIVerb", "Could not allocate shared buffer!");
    }

    reverb.Init(reverb_buffer);
    reverb.set_amount(0.5f);
    reverb.set_lp(0.5f);
    reverb.set_lfo2_freq(0.1f);
    reverb.set_lfo1_freq(0.2f);
    reverb.set_time(0.2f);
    reverb.set_diffusion(0.5f);
}

void ctagSoundProcessorMIVerb::Process(const ProcessData &data) {

    float fTime = time / 4095.f;
    float fDiffusion = diffusion / 4095.f;
    float fLp = lp / 4095.f;
    float fGain = in_gain / 4095.f;
    float fAmount = amount / 4095.f;
    float lfo1 = lfo1_f / 4095.f;
    float lfo2 = lfo2_f / 4095.f;

    if(cv_time != -1){
        fTime = HELPERS::fastfabs(data.cv[cv_time]);
    }
    if(cv_diffusion != -1){
        fDiffusion = HELPERS::fastfabs(data.cv[cv_diffusion]);
    }
    if(cv_lp != -1){
        fLp = HELPERS::fastfabs(data.cv[cv_lp]);
    }
    if(cv_in_gain != -1){
        fGain = HELPERS::fastfabs(data.cv[cv_in_gain]);
    }
    if(cv_amount != -1){
        fAmount = HELPERS::fastfabs(data.cv[cv_amount]);
    }
    if(cv_lfo1_f != -1){
        lfo1 = HELPERS::fastfabs(data.cv[cv_lfo1_f]);
    }
    if(cv_lfo2_f != -1){
        lfo2 = HELPERS::fastfabs(data.cv[cv_lfo2_f]);
    }

    reverb.set_diffusion(fDiffusion);
    reverb.set_input_gain(fGain);
    reverb.set_amount(fAmount);
    reverb.set_lp(fLp);
    reverb.set_time(fTime);
    reverb.set_lfo1_freq(lfo1);
    reverb.set_lfo2_freq(lfo2);

    float left[bufSz], right[bufSz];
    for(int i=0;i<bufSz;i++){
        left[i] = data.buf[i*2];
        right[i] = data.buf[i*2 + 1];
    }

    reverb.Process(left, right, bufSz);

    for(int i=0;i<bufSz;i++){
        data.buf[i*2] = left[i];
        data.buf[i*2 + 1] = right[i];
    }
}

ctagSoundProcessorMIVerb::~ctagSoundProcessorMIVerb() {
    heap_caps_free(reverb_buffer);
}

const char *ctagSoundProcessorMIVerb::GetCStrID() const {
    return id.c_str();
}


void ctagSoundProcessorMIVerb::setParamValueInternal(const string& id, const string& key, const int val) {
// autogenerated code here
// sectionCpp0
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
if(id.compare("in_gain") == 0){
	if(key.compare("current") == 0){
		in_gain = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_in_gain = val;
	}
	return;
}
if(id.compare("diffusion") == 0){
	if(key.compare("current") == 0){
		diffusion = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_diffusion = val;
	}
	return;
}
if(id.compare("lp") == 0){
	if(key.compare("current") == 0){
		lp = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_lp = val;
	}
	return;
}
if(id.compare("lfo1_f") == 0){
	if(key.compare("current") == 0){
		lfo1_f = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_lfo1_f = val;
	}
	return;
}
if(id.compare("lfo2_f") == 0){
	if(key.compare("current") == 0){
		lfo2_f = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_lfo2_f = val;
	}
	return;
}
// sectionCpp0


}

void ctagSoundProcessorMIVerb::loadPresetInternal() {
// autogenerated code here
// sectionCpp1
time = model->GetParamValue("time", "current");
cv_time = model->GetParamValue("time", "cv");
amount = model->GetParamValue("amount", "current");
cv_amount = model->GetParamValue("amount", "cv");
in_gain = model->GetParamValue("in_gain", "current");
cv_in_gain = model->GetParamValue("in_gain", "cv");
diffusion = model->GetParamValue("diffusion", "current");
cv_diffusion = model->GetParamValue("diffusion", "cv");
lp = model->GetParamValue("lp", "current");
cv_lp = model->GetParamValue("lp", "cv");
lfo1_f = model->GetParamValue("lfo1_f", "current");
cv_lfo1_f = model->GetParamValue("lfo1_f", "cv");
lfo2_f = model->GetParamValue("lfo2_f", "current");
cv_lfo2_f = model->GetParamValue("lfo2_f", "cv");
// sectionCpp1


}
