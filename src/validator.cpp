#include "validator.hpp"

bool PEValidator::validate(const MappedFile &mf) const
{
	if (mf.getSize() < 64)
		return false;

	const std::byte *data = mf.getData();

	// DOS header 'MZ'
	if (data[0] != std::byte{'M'} || data[1] != std::byte{'Z'})
		return false;

	// e_lfanew
	uint32_t e_lfanew = *reinterpret_cast<const uint32_t *>(data + 0x3C);
	if (e_lfanew >= mf.getSize() - 4)
		return false;

	// NT header 'PE\0\0'
	const std::byte *nt = data + e_lfanew;
	if (nt[0] != std::byte{'P'} || nt[1] != std::byte{'E'} ||
	    nt[2] != std::byte{0} || nt[3] != std::byte{0})
		return false;

	return true; // basic sanity check passed
}

bool ZIPValidator::validate(const MappedFile &mf) const
{
	if (mf.getSize() < 4)
		return false;
	const std::byte *data = mf.getData();

	// Check first local file header
	if (data[0] != std::byte{0x50} || data[1] != std::byte{0x4B} ||
	    data[2] != std::byte{0x03} || data[3] != std::byte{0x04})
		return false;

	// End of central directory at the end (simplified)
	if (mf.getSize() < 22)
		return false; // EOCD minimum size
	const std::byte *eocd = data + mf.getSize() - 22;
	if (eocd[0] != std::byte{0x50} || eocd[1] != std::byte{0x4B} ||
	    eocd[2] != std::byte{0x05} || eocd[3] != std::byte{0x06})
		return false;

	return true;
}

// ----------------- PDF Validator -----------------
bool PDFValidator::validate(const MappedFile &mf) const
{
	const size_t size = mf.getSize();
	if (size < 8)
		return false;

	const std::byte *data = mf.getData();

	// -------------------------------------------------
	// 1. Header check: "%PDF-"
	// -------------------------------------------------
	if (data[0] != std::byte{'%'} || data[1] != std::byte{'P'} ||
	    data[2] != std::byte{'D'} || data[3] != std::byte{'F'} ||
	    data[4] != std::byte{'-'})
		return false;

	// -------------------------------------------------
	// 2. Search for %%EOF near the end
	//    (PDF spec: EOF marker must exist, but not fixed offset)
	// -------------------------------------------------
	constexpr size_t MAX_SCAN = 1024;
	size_t scanSize = std::min(size, MAX_SCAN);

	const char *tail = reinterpret_cast<const char *>(data + size - scanSize);

	std::string_view tailView(tail, scanSize);

	if (tailView.find("%%EOF") == std::string_view::npos)
		return false;

	// -------------------------------------------------
	// 3. xref sanity (optional but strong)
	// -------------------------------------------------
	if (tailView.find("xref") == std::string_view::npos &&
	    tailView.find("/XRef") == std::string_view::npos)
		return false;

	return true;
}

// ----------------- ISO Validator -----------------
bool ISOValidator::validate(const MappedFile &mf) const
{
	if (mf.getSize() < 0x8001 + 5)
		return false;

	const std::byte *data = mf.getData();
	// Primary Volume Descriptor
	if (data[0x8001] != std::byte{'C'} || data[0x8002] != std::byte{'D'} ||
	    data[0x8003] != std::byte{'0'} || data[0x8004] != std::byte{'0'} ||
	    data[0x8005] != std::byte{'1'})
		return false;

	return true;
}

std::unordered_map<std::string, std::shared_ptr<FileValidator>> validators = {
    {"pe", std::make_shared<PEValidator>()},
    {"zip", std::make_shared<ZIPValidator>()},
    {"pdf", std::make_shared<PDFValidator>()},
    {"iso", std::make_shared<ISOValidator>()}};

//
// ----------------- Detection -----------------
std::vector<std::string>
detectFiletypes(const MappedFile &mf,
                const std::vector<const Signature *> &hits)
{
	std::unordered_map<std::string, int> scores;

	// Sum weights per filetype
	for (auto sig : hits)
		scores[sig->filetype] += sig->weight;

	std::vector<std::string> confirmed;

	for (auto &[ftype, _] : scores)
	{
		auto it = validators.find(ftype);
		if (it != validators.end() && it->second->validate(mf))
			confirmed.push_back(ftype);
	}

	return confirmed;
}
