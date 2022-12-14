
#include "./CheckpointWriter.h"

namespace outputWriter
{
    CheckpointWriter::CheckpointWriter()
    {
        _logicLogger = spdlog::get("output_logger");
        _memoryLogger = spdlog::get("memory_logger");
        _memoryLogger->info("CheckpointWriter generated!");
    }

    CheckpointWriter::~CheckpointWriter()
    {
        _memoryLogger->info("CheckpointWriter destructed!");
    }

    void CheckpointWriter::writeCheckpoint(ParticleContainer *particleContainer)
    {
    }
}