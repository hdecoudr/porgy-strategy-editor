/**
 *
 * This file is part of Porgy (http://tulip.labri.fr/TulipDrupal/?q=porgy)
 *
 * from LaBRI, University of Bordeaux, Inria and King's College London
 *
 * Porgy is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Porgy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this Porgy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GENERALPURPOSESYNTAXHIGHLIGHTINGRULES_H
#define GENERALPURPOSESYNTAXHIGHLIGHTINGRULES_H

#include <map>
#include <vector>

class HighlightingRule;

class GeneralPurposeSyntaxHighlightingRules
{
public:
    GeneralPurposeSyntaxHighlightingRules();
    ~GeneralPurposeSyntaxHighlightingRules();

public:
    void appendRule(const std::string& key, const HighlightingRule& rule);
    
public:
    std::map<std::string, std::vector<HighlightingRule>>::iterator begin();
    std::map<std::string, std::vector<HighlightingRule>>::iterator end();

public:
    std::size_t size() const;
    std::size_t size(const std::string& key) const;

public:
    std::vector<HighlightingRule> operator [](const std::string& key) const;
    std::vector<HighlightingRule> operator [](const char* key) const;
    std::vector<HighlightingRule>& operator [](const std::string& key);
    std::vector<HighlightingRule>& operator [](const char* key);

private:
    std::map<std::string, std::vector<HighlightingRule>> highlightingRules;
};

#endif // GENERALPURPOSESYNTAXHIGHLIGHTINGRULES_H
