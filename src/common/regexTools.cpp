/*
 * Copyright 2011 Emmanuel Engelhart <kelson@kiwix.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "regexTools.h"

std::map<std::string, RegexMatcher*> regexCache;

RegexMatcher *buildRegex(const std::string &regex) {
  RegexMatcher *matcher;
  std::map<std::string, RegexMatcher*>::iterator itr = regexCache.find(regex);
  
  /* Regex is in cache */
  if (itr != regexCache.end()) {
    matcher = itr->second;
  }

  /* Regex needs to be parsed (and cached) */
  else {
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString uregex = UnicodeString(regex.c_str());
    matcher = new RegexMatcher(uregex, UREGEX_CASE_INSENSITIVE, status);
    regexCache[regex] = matcher;
  }

  return matcher;
}

/* todo */
void freeRegexCache() {
}

bool matchRegex(const std::string &content, const std::string &regex) {
  ucnv_setDefaultName("UTF-8");
  UnicodeString ucontent = UnicodeString(content.c_str());
  RegexMatcher *matcher = buildRegex(regex);
  matcher->reset(ucontent);
  return matcher->find();
}
