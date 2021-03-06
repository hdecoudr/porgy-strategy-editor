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

#include "generalpurposesyntaxhighlightingrules.h"
#include "highlightingrule.h"


/****************************************************************************************
 ****************************************************************************************
 *
 * Constructor.
 *
 ****************************************************************************************/
GeneralPurposeSyntaxHighlightingRules::GeneralPurposeSyntaxHighlightingRules() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Destructor.
 *
 ****************************************************************************************/
GeneralPurposeSyntaxHighlightingRules::~GeneralPurposeSyntaxHighlightingRules() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Add rule.
 *
 ****************************************************************************************/
void GeneralPurposeSyntaxHighlightingRules::appendRule(const std::string& key, const HighlightingRule& rule)
{
    highlightingRules[key].push_back(rule);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get map begin.
 *
 ****************************************************************************************/
std::map<std::string, std::vector<HighlightingRule>>::iterator GeneralPurposeSyntaxHighlightingRules::begin()
{
    return highlightingRules.begin();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get map end.
 *
 ****************************************************************************************/
std::map<std::string, std::vector<HighlightingRule>>::iterator GeneralPurposeSyntaxHighlightingRules::end()
{
    return highlightingRules.end();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get map size.
 *
 ****************************************************************************************/
std::size_t GeneralPurposeSyntaxHighlightingRules::size() const
{
    return highlightingRules.size();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get vector size.
 *
 ****************************************************************************************/
std::size_t GeneralPurposeSyntaxHighlightingRules::size(const std::string& key) const
{
    if(highlightingRules.find(key) == highlightingRules.end())
    {
        return -1;
    }

    return highlightingRules.at(key).size();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get a vector of rules.
 *
 ****************************************************************************************/
std::vector<HighlightingRule> GeneralPurposeSyntaxHighlightingRules::operator ()(const std::string& key) const
{
    if(highlightingRules.find(key) == highlightingRules.end())
    {
        return std::vector<HighlightingRule>();
    }

    return highlightingRules.at(key);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Set a rule.
 *
 ****************************************************************************************/
std::vector<HighlightingRule>& GeneralPurposeSyntaxHighlightingRules::operator ()(const std::string& key)
{
    return highlightingRules[key];
}
