/* runperl.cpp: running scripts

   Copyright (C) 1996-2018 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "config.h"

#include <miktex/Core/CommandLineBuilder>
#include <miktex/Core/Paths>

#include "internal.h"

#include "Session/SessionImpl.h"

using namespace std;

using namespace MiKTeX::Core;

int SessionImpl::RunScript(const string& scriptEngine, const string& scriptEngineArgument, int argc, const char** argv)
{
  MIKTEX_ASSERT(argc > 0);

  // determine script name
  PathName name = PathName(argv[0]).GetFileNameWithoutExtension();

  // find engine
  PathName engine;
  PathName scriptEngineWithExeSuffix = scriptEngine;
#if defined(MIKTEX_WINDOWS)
  scriptEngineWithExeSuffix.SetExtension(MIKTEX_EXE_FILE_SUFFIX, false);
#endif
  if (!Utils::FindProgram(scriptEngineWithExeSuffix.ToString(), engine))
  {
    MIKTEX_FATAL_ERROR_5(
      T_("The script engine could not be found."),
      T_("MiKTeX could not find the script engine '{scriptEngine}' which is required to execute '{scriptName}'."),
      T_("Make sure '{scriptEngine}' is installed on your system."),
      "script-engine-not-found",
      "scriptEngine", scriptEngineWithExeSuffix.ToString(),
      "scriptName", name.ToString());
  }

  // get relative script path
  PathName scriptsIni;
  if (!FindFile(MIKTEX_PATH_SCRIPTS_INI, MIKTEX_PATH_TEXMF_PLACEHOLDER, scriptsIni))
  {
    MIKTEX_FATAL_ERROR(T_("The script configuration file cannot be found."));
  }
  unique_ptr<Cfg> config(Cfg::Create());
  config->Read(scriptsIni, true);
  string relScriptPath;
  if (!config->TryGetValueAsString(scriptEngine, name.ToString(), relScriptPath))
  {
    MIKTEX_FATAL_ERROR_2(T_("The script is not registered."), "scriptEngine", scriptEngine, "scriptName", name.ToString());
  }

  // find script
  PathName scriptPath;
  if (!FindFile(relScriptPath, MIKTEX_PATH_TEXMF_PLACEHOLDER, scriptPath))
  {
    MIKTEX_FATAL_ERROR_2(T_("The script could not be found."), "scriptEngine", scriptEngine, "scriptName", name.ToString(), "path", relScriptPath);
  }

  vector<string> args{ scriptEngine };
  vector<string> scriptEngineOptions;
  if (config->TryGetValueAsStringVector(scriptEngine, name.ToString() + "." + scriptEngine + "." + "options" + "[]", scriptEngineOptions))
  {
    args.insert(args.end(), scriptEngineOptions.begin(), scriptEngineOptions.end());
  }
  if (!scriptEngineArgument.empty())
  {
    args.push_back(scriptEngineArgument);
  }
  args.push_back(scriptPath.ToString());
  for(int idx = 1; idx < argc; ++idx)
  {
    args.push_back(argv[idx]);
  }

  int exitCode;

  Process::Run(engine, args, nullptr, &exitCode, nullptr);

  return exitCode;
}

#define PERL "perl"

int SessionImpl::RunPerl(int argc, const char** argv)
{
  return RunScript(PERL, "", argc, argv);
}

#define PYTHON "python"

int SessionImpl::RunPython(int argc, const char** argv)
{
  return RunScript(PYTHON, "", argc, argv);
}

#define JAVA "java"

int SessionImpl::RunJava(int argc, const char** argv)
{
  return RunScript(JAVA, "-jar", argc, argv);
}
