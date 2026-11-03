// The engine interfaces the script runner can put behind the probe's calls.
#pragma once

#include "probe.hpp"

namespace libseq {
class Engine;
}

namespace ttsrun {

/// TTSSEQ.DLL 1.01.
void defineApi101(Probe& p, libseq::Engine& e);

} // namespace ttsrun
