/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.ws.commons.schema.utils;

import java.util.StringTokenizer;
import java.util.Vector;

/**
 * @author mukund
 */

public class Tokenizer {

    /**
     * Cannot create new Tokenizer
     */
    private Tokenizer() {
    }

    public static final String[] tokenize(String in, String delim) {
        Vector tmp = new Vector();
        StringTokenizer stk = new StringTokenizer(in, delim);
        while (stk.hasMoreTokens()) {
            String str = stk.nextToken();
            if (str.trim().length() != 0)
                tmp.addElement(str);
        }
        return (String[]) tmp.toArray(new String[tmp.size()]);
    }

    public static final String[] firstToken(String in, String delim) {
        int x = in.indexOf(delim);
        if (x == -1)
            return new String[]{in, ""};
        return new String[]{in.substring(0, x),
                            in.substring(x + delim.length(), in.length())};
    }

    public static final String[] lastToken(String in, String delim) {
        int x = in.lastIndexOf(delim);
        if (x == -1)
            return new String[]{"", in};
        return new String[]{in.substring(0, x),
                            in.substring(x + delim.length(), in.length())};
    }
}
