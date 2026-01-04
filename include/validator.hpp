#pragma once
#include "mapped_file.hpp"
#include "signature.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// ----------------- Validator base -----------------
class FileValidator {
  public:
	virtual bool validate(const MappedFile &mf) const = 0;
	virtual ~FileValidator() = default;
};

// ----------------- PE Validator -----------------
class PEValidator : public FileValidator {
  public:
	bool validate(const MappedFile &mf) const override;
};

// ----------------- ZIP Validator -----------------
class ZIPValidator : public FileValidator {
  public:
	bool validate(const MappedFile &mf) const override;
};

// ----------------- PDF Validator -----------------
class PDFValidator : public FileValidator {
  public:
	bool validate(const MappedFile &mf) const override;
};

// ----------------- ISO Validator -----------------
class ISOValidator : public FileValidator {
  public:
	bool validate(const MappedFile &mf) const override;
};

//
// ----------------- Detection -----------------
std::vector<std::string>
detectFiletypes(const MappedFile &mf,
                const std::vector<const Signature *> &hits);
