/* xoreos-tools - Tools to help with xoreos development
 *
 * xoreos-tools is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos-tools is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos-tools. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Dump TLKs into XML files.
 */

#include "src/common/readstream.h"
#include "src/common/writestream.h"

#include "src/aurora/language.h"
#include "src/aurora/talktable.h"

#include "src/xml/tlkdumper.h"
#include "src/xml/xmlwriter.h"

namespace XML {

TLKDumper::TLKDumper() {
}

TLKDumper::~TLKDumper() {
}

void TLKDumper::dump(Common::WriteStream &output, Common::SeekableReadStream *input,
                     Common::Encoding encoding) {

	Aurora::TalkTable *tlk = Aurora::TalkTable::load(input, encoding);
	if (!tlk)
		return;

	const uint32 languageID = tlk->getLanguageID();

	XMLWriter xml(output);

	xml.openTag("tlk");
	if (languageID != Aurora::kLanguageInvalid)
		xml.addProperty("language", Common::UString::format("%u", languageID));
	xml.breakLine();

	const std::list<uint32> &strRefs = tlk->getStrRefs();

	for (std::list<uint32>::const_iterator s = strRefs.begin(); s != strRefs.end(); ++s) {
		const uint32 strRef = *s;

		Common::UString str, sound;
		tlk->getString(strRef, str, sound);

		if (str.empty())
			continue;

		xml.openTag("string");
		xml.addProperty("id", Common::UString::format("%u", strRef));

		if (!sound.empty())
			xml.addProperty("sound", sound);

		xml.setContents(str);

		xml.closeTag();
		xml.breakLine();
	}

	xml.closeTag();
	xml.breakLine();

	xml.flush();

	delete tlk;
}

} // End of namespace XML
