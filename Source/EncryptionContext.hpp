#pragma once

namespace conv
{
    class EncryptionContext
    {
    public:
        EncryptionContext();
        ~EncryptionContext();
        EncryptionContext(const EncryptionContext& other) = delete;
        EncryptionContext(EncryptionContext&& other) noexcept = delete;
        EncryptionContext& operator=(const EncryptionContext& other) = delete;
        EncryptionContext& operator=(EncryptionContext&& other) noexcept = delete;
    private:

    };
}