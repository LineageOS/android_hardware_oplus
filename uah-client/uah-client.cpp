#include <stdio.h>

extern "C" {
    void uahInit() {}
    void UahEventAcquire() {}
    void UahRelease() {}
    void UahNotify() {}
    void UahResAcquire() {}
    void UahResStateRequest() {}
    void UahRuleCtl() {}
    void UahGetHistory() {}
    void UahGetPMStatus() {}
}
