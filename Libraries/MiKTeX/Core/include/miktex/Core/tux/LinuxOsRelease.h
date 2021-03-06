/* LinuxOsRelease.h:                                    -*- C++ -*-

   Copyright (C) 2018 Christian Schenk

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

#pragma once

#include <miktex/Core/config.h>

#include <string>

class LinuxOsRelease
{
public:
  std::string name;

public:
  std::string version;

public:
  std::string id;

public:
  std::string id_like;

public:
  std::string version_codename;

public:
  std::string version_id;

public:
  std::string pretty_name;

public:
  std::string ansi_color;

public:
  std::string cpe_name;

public:
  std::string home_url;

public:
  std::string support_url;

public:
  std::string bug_report_url;

public:
  std::string privacy_policy_url;

public:
  std::string build_id;

public:
  std::string variant;

public:
  std::string variant_id;
  
public:
  static MIKTEXCORECEEAPI(LinuxOsRelease) Get();
};
