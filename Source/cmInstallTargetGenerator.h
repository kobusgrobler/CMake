/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#ifndef cmInstallTargetGenerator_h
#define cmInstallTargetGenerator_h

#include "cmConfigure.h" // IWYU pragma: keep

#include "cmInstallGenerator.h"
#include "cmListFileCache.h"
#include "cmScriptGenerator.h"

#include <iosfwd>
#include <string>
#include <vector>

class cmGeneratorTarget;
class cmLocalGenerator;

/** \class cmInstallTargetGenerator
 * \brief Generate target installation rules.
 */
class cmInstallTargetGenerator : public cmInstallGenerator
{
public:
  cmInstallTargetGenerator(
    std::string const& targetName, const char* dest, bool implib,
    const char* file_permissions,
    std::vector<std::string> const& configurations, const char* component,
    MessageLevel message, bool exclude_from_all, bool optional,
    cmListFileBacktrace const& backtrace = cmListFileBacktrace());
  ~cmInstallTargetGenerator() override;

  /** Select the policy for installing shared library linkable name
      symlinks.  */
  enum NamelinkModeType
  {
    NamelinkModeNone,
    NamelinkModeOnly,
    NamelinkModeSkip
  };
  void SetNamelinkMode(NamelinkModeType mode) { this->NamelinkMode = mode; }
  NamelinkModeType GetNamelinkMode() const { return this->NamelinkMode; }

  std::string GetInstallFilename(const std::string& config) const;

  void GetInstallObjectNames(std::string const& config,
                             std::vector<std::string>& objects) const;

  enum NameType
  {
    NameNormal,
    NameImplib,
    NameSO,
    NameReal
  };

  static std::string GetInstallFilename(const cmGeneratorTarget* target,
                                        const std::string& config,
                                        NameType nameType = NameNormal);

  void Compute(cmLocalGenerator* lg) override;

  cmGeneratorTarget* GetTarget() const { return this->Target; }

  bool IsImportLibrary() const { return this->ImportLibrary; }

  std::string GetDestination(std::string const& config) const;

  cmListFileBacktrace const& GetBacktrace() const { return this->Backtrace; }

protected:
  void GenerateScript(std::ostream& os) override;
  void GenerateScriptForConfig(std::ostream& os, const std::string& config,
                               Indent indent) override;
  void GenerateScriptForConfigObjectLibrary(std::ostream& os,
                                            const std::string& config,
                                            Indent indent);
  typedef void (cmInstallTargetGenerator::*TweakMethod)(std::ostream&, Indent,
                                                        const std::string&,
                                                        std::string const&);
  void AddTweak(std::ostream& os, Indent indent, const std::string& config,
                std::string const& file, TweakMethod tweak);
  void AddTweak(std::ostream& os, Indent indent, const std::string& config,
                std::vector<std::string> const& files, TweakMethod tweak);
  std::string GetDestDirPath(std::string const& file);
  void PreReplacementTweaks(std::ostream& os, Indent indent,
                            const std::string& config,
                            std::string const& file);
  void PostReplacementTweaks(std::ostream& os, Indent indent,
                             const std::string& config,
                             std::string const& file);
  void AddInstallNamePatchRule(std::ostream& os, Indent indent,
                               const std::string& config,
                               const std::string& toDestDirPath);
  void AddChrpathPatchRule(std::ostream& os, Indent indent,
                           const std::string& config,
                           std::string const& toDestDirPath);
  void AddRPathCheckRule(std::ostream& os, Indent indent,
                         const std::string& config,
                         std::string const& toDestDirPath);

  void AddStripRule(std::ostream& os, Indent indent,
                    const std::string& toDestDirPath);
  void AddRanlibRule(std::ostream& os, Indent indent,
                     const std::string& toDestDirPath);
  void AddUniversalInstallRule(std::ostream& os, Indent indent,
                               const std::string& toDestDirPath);

  std::string TargetName;
  cmGeneratorTarget* Target;
  std::string FilePermissions;
  NamelinkModeType NamelinkMode;
  bool ImportLibrary;
  bool Optional;
  cmListFileBacktrace Backtrace;
};

#endif
