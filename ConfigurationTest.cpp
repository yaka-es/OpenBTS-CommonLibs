/* CommonLibs/ConfigurationTest.cpp */
/*-
 * Copyright 2009, 2010 Free Software Foundation, Inc.
 * Copyright 2010 Kestrel Signal Processing, Inc.
 * Copyright 2014 Range Networks, Inc.
 *
 * This software is distributed under the terms of the GNU Affero Public License.
 * See the COPYING file in the main directory for details.
 *
 * This use of this software may be subject to additional restrictions.
 * See the LEGAL file in the main directory for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>

#include "Configuration.h"

using namespace std;

ConfigurationKeyMap getConfigurationKeys();
ConfigurationTable *gConfigObject;

void purgeConfig(void *, int, char const *, char const *, sqlite3_int64)
{
	// cout << "update hook" << endl;
	gConfigTable.purge();
}

int main(int argc, char **argv)
{
	gConfigObject = new ConfigurationTable("exampleconfig.db", "test", getConfigurationKeys());

	gConfigTable.setUpdateHook(purgeConfig);

	const char *keys[5] = {"key1", "key2", "key3", "key4", "key5"};

	for (int i = 0; i < 5; i++) {
		gConfigTable.set(keys[i], i);
	}

	for (int i = 0; i < 5; i++) {
		cout << "table[" << keys[i] << "]=" << gConfigTable.getStr(keys[i]) << endl;
		cout << "table[" << keys[i] << "]=" << gConfigTable.getNum(keys[i]) << endl;
	}

	for (int i = 0; i < 5; i++) {
		cout << "defined table[" << keys[i] << "]=" << gConfigTable.defines(keys[i]) << endl;
	}

	gConfigTable.set("key5", "100 200 300  400 ");
	std::vector<unsigned> vect = gConfigTable.getVector("key5");
	cout << "vect length " << vect.size() << ": ";
	for (unsigned i = 0; i < vect.size(); i++)
		cout << " " << vect[i];
	cout << endl;
	std::vector<string> svect = gConfigTable.getVectorOfStrings("key5");
	cout << "vect length " << svect.size() << ": ";
	for (unsigned i = 0; i < svect.size(); i++)
		cout << " " << svect[i] << ":";
	cout << endl;

	cout << "bool " << gConfigTable.getBool("booltest") << endl;
	gConfigTable.set("booltest", 1);
	cout << "bool " << gConfigTable.getBool("booltest") << endl;
	gConfigTable.set("booltest", 0);
	cout << "bool " << gConfigTable.getBool("booltest") << endl;

	gConfigTable.getStr("newstring");
	gConfigTable.getNum("numnumber");

	SimpleKeyValue pairs;
	pairs.addItems(" a=1 b=34 dd=143 ");
	cout << pairs.get("a") << endl;
	cout << pairs.get("b") << endl;
	cout << pairs.get("dd") << endl;

	gConfigTable.set("fkey", "123.456");
	float fval = gConfigTable.getFloat("fkey");
	cout << "fkey " << fval << endl;

	cout << "search fkey:" << endl;
	gConfigTable.find("fkey", cout);
	cout << "search fkey:" << endl;
	gConfigTable.find("fkey", cout);
	gConfigTable.remove("fkey");
	cout << "search fkey:" << endl;
	gConfigTable.find("fkey", cout);

	try {
		gConfigTable.getNum("supposedtoabort");
	} catch (ConfigurationTableKeyNotFound) {
		cout << "ConfigurationTableKeyNotFound exception successfully caught." << endl;
	}

	delete gConfigObject;

	return 0;
}

ConfigurationKeyMap getConfigurationKeys()
{
	ConfigurationKeyMap map;
	ConfigurationKey *tmp;

	tmp = new ConfigurationKey(
		"booltest", "0", "", ConfigurationKey::DEVELOPER, ConfigurationKey::BOOLEAN, "", false, "");
	map[tmp->getName()] = *tmp;
	delete tmp;

	tmp = new ConfigurationKey(
		"numnumber", "42", "", ConfigurationKey::DEVELOPER, ConfigurationKey::VALRANGE, "0-100", false, "");
	map[tmp->getName()] = *tmp;
	delete tmp;

	tmp = new ConfigurationKey("newstring", "new string value", "", ConfigurationKey::DEVELOPER,
		ConfigurationKey::STRING, "", false, "");
	map[tmp->getName()] = *tmp;
	delete tmp;

	return map;
}
