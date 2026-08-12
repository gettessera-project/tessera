// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_FEES_H
#define TESSERA_UTIL_FEES_H

/* Used to determine type of fee estimation requested */
enum class FeeEstimateMode {
    UNSET,        //!< Use default settings based on other criteria
    ECONOMICAL,   //!< Force estimateSmartFee to use non-conservative estimates
    CONSERVATIVE, //!< Force estimateSmartFee to use conservative estimates
};

#endif // TESSERA_UTIL_FEES_H
