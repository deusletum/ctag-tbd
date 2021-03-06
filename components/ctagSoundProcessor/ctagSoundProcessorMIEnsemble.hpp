#include <atomic>
#include "ctagSoundProcessor.hpp"
#include "mifx/ensemble.h"

namespace CTAG {
    namespace SP {
        class ctagSoundProcessorMIEnsemble : public ctagSoundProcessor {
        public:
            virtual void Process(const ProcessData &);

            virtual ~ctagSoundProcessorMIEnsemble();

            ctagSoundProcessorMIEnsemble();

            virtual const char *GetCStrID() const;

        private:
            void setParamValueInternal(const string &id, const string &key, const int val) override;
            void loadPresetInternal() override;

// autogenerated code here
// sectionHpp
const string id = "MIEnsemble";
void setIsStereo(){isStereo = true;}
atomic<int32_t> depth, cv_depth;
atomic<int32_t> amount, cv_amount;
// sectionHpp


            // private attributes could go here
            mifx::Ensemble fx;
            float *fx_buffer;
        };
    }
}