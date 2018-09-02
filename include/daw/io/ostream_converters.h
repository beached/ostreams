// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <limits>
#include <string>

#include <daw/daw_exception.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

#include "ostream_helpers.h"
#include "static_string.h"

namespace ostream_converters {
	namespace impl {
		template<typename T, typename Result = daw::remove_cvref_t<T>>
		constexpr Result num_digits( T value ) noexcept {
			static_assert( daw::is_integral_v<Result>,
			               "Result must be an integer type" );
			if( value == 0 ) {
				return static_cast<Result>( 1 );
			}
			Result result = 0;
			while( value != 0 ) {
				value /= 10;
				++result;
			}
			return result;
		}

		template<typename Result, std::enable_if_t<daw::is_arithmetic_v<Result>,
		                                           std::nullptr_t> = nullptr>
		constexpr Result pow10( int16_t n ) noexcept {
			daw::exception::dbg_precondition_check(
			  n >= 0, "Only positive values of n supported" );

			switch( n ) {
			case 0:
				return static_cast<Result>( 1.0e+0 );
			case 1:
				return static_cast<Result>( 1.0e+1 );
			case 2:
				return static_cast<Result>( 1.0e+2 );
			case 3:
				return static_cast<Result>( 1.0e+3 );
			case 4:
				return static_cast<Result>( 1.0e+4 );
			case 5:
				return static_cast<Result>( 1.0e+5 );
			case 6:
				return static_cast<Result>( 1.0e+6 );
			case 7:
				return static_cast<Result>( 1.0e+7 );
			case 8:
				return static_cast<Result>( 1.0e+8 );
			case 9:
				return static_cast<Result>( 1.0e+9 );
			case 10:
				return static_cast<Result>( 1.0e+10 );
			case 11:
				return static_cast<Result>( 1.0e+11 );
			case 12:
				return static_cast<Result>( 1.0e+12 );
			case 13:
				return static_cast<Result>( 1.0e+13 );
			case 14:
				return static_cast<Result>( 1.0e+14 );
			case 15:
				return static_cast<Result>( 1.0e+15 );
			case 16:
				return static_cast<Result>( 1.0e+16 );
			case 17:
				return static_cast<Result>( 1.0e+17 );
			case 18:
				return static_cast<Result>( 1.0e+18 );
			case 19:
				return static_cast<Result>( 1.0e+19 );
			case 20:
				return static_cast<Result>( 1.0e+20 );
			case 21:
				return static_cast<Result>( 1.0e+21 );
			case 22:
				return static_cast<Result>( 1.0e+22 );
			case 23:
				return static_cast<Result>( 1.0e+23 );
			case 24:
				return static_cast<Result>( 1.0e+24 );
			case 25:
				return static_cast<Result>( 1.0e+25 );
			case 26:
				return static_cast<Result>( 1.0e+26 );
			case 27:
				return static_cast<Result>( 1.0e+27 );
			case 28:
				return static_cast<Result>( 1.0e+28 );
			case 29:
				return static_cast<Result>( 1.0e+29 );
			case 30:
				return static_cast<Result>( 1.0e+30 );
			case 31:
				return static_cast<Result>( 1.0e+31 );
			case 32:
				return static_cast<Result>( 1.0e+32 );
			case 33:
				return static_cast<Result>( 1.0e+33 );
			case 34:
				return static_cast<Result>( 1.0e+34 );
			case 35:
				return static_cast<Result>( 1.0e+35 );
			case 36:
				return static_cast<Result>( 1.0e+36 );
			case 37:
				return static_cast<Result>( 1.0e+37 );
			case 38:
				return static_cast<Result>( 1.0e+38 );
			case 39:
				return static_cast<Result>( 1.0e+39 );
			case 40:
				return static_cast<Result>( 1.0e+40 );
			case 41:
				return static_cast<Result>( 1.0e+41 );
			case 42:
				return static_cast<Result>( 1.0e+42 );
			case 43:
				return static_cast<Result>( 1.0e+43 );
			case 44:
				return static_cast<Result>( 1.0e+44 );
			case 45:
				return static_cast<Result>( 1.0e+45 );
			case 46:
				return static_cast<Result>( 1.0e+46 );
			case 47:
				return static_cast<Result>( 1.0e+47 );
			case 48:
				return static_cast<Result>( 1.0e+48 );
			case 49:
				return static_cast<Result>( 1.0e+49 );
			case 50:
				return static_cast<Result>( 1.0e+50 );
			case 51:
				return static_cast<Result>( 1.0e+51 );
			case 52:
				return static_cast<Result>( 1.0e+52 );
			case 53:
				return static_cast<Result>( 1.0e+53 );
			case 54:
				return static_cast<Result>( 1.0e+54 );
			case 55:
				return static_cast<Result>( 1.0e+55 );
			case 56:
				return static_cast<Result>( 1.0e+56 );
			case 57:
				return static_cast<Result>( 1.0e+57 );
			case 58:
				return static_cast<Result>( 1.0e+58 );
			case 59:
				return static_cast<Result>( 1.0e+59 );
			case 60:
				return static_cast<Result>( 1.0e+60 );
			case 61:
				return static_cast<Result>( 1.0e+61 );
			case 62:
				return static_cast<Result>( 1.0e+62 );
			case 63:
				return static_cast<Result>( 1.0e+63 );
			case 64:
				return static_cast<Result>( 1.0e+64 );
			case 65:
				return static_cast<Result>( 1.0e+65 );
			case 66:
				return static_cast<Result>( 1.0e+66 );
			case 67:
				return static_cast<Result>( 1.0e+67 );
			case 68:
				return static_cast<Result>( 1.0e+68 );
			case 69:
				return static_cast<Result>( 1.0e+69 );
			case 70:
				return static_cast<Result>( 1.0e+70 );
			case 71:
				return static_cast<Result>( 1.0e+71 );
			case 72:
				return static_cast<Result>( 1.0e+72 );
			case 73:
				return static_cast<Result>( 1.0e+73 );
			case 74:
				return static_cast<Result>( 1.0e+74 );
			case 75:
				return static_cast<Result>( 1.0e+75 );
			case 76:
				return static_cast<Result>( 1.0e+76 );
			case 77:
				return static_cast<Result>( 1.0e+77 );
			case 78:
				return static_cast<Result>( 1.0e+78 );
			case 79:
				return static_cast<Result>( 1.0e+79 );
			case 80:
				return static_cast<Result>( 1.0e+80 );
			case 81:
				return static_cast<Result>( 1.0e+81 );
			case 82:
				return static_cast<Result>( 1.0e+82 );
			case 83:
				return static_cast<Result>( 1.0e+83 );
			case 84:
				return static_cast<Result>( 1.0e+84 );
			case 85:
				return static_cast<Result>( 1.0e+85 );
			case 86:
				return static_cast<Result>( 1.0e+86 );
			case 87:
				return static_cast<Result>( 1.0e+87 );
			case 88:
				return static_cast<Result>( 1.0e+88 );
			case 89:
				return static_cast<Result>( 1.0e+89 );
			case 90:
				return static_cast<Result>( 1.0e+90 );
			case 91:
				return static_cast<Result>( 1.0e+91 );
			case 92:
				return static_cast<Result>( 1.0e+92 );
			case 93:
				return static_cast<Result>( 1.0e+93 );
			case 94:
				return static_cast<Result>( 1.0e+94 );
			case 95:
				return static_cast<Result>( 1.0e+95 );
			case 96:
				return static_cast<Result>( 1.0e+96 );
			case 97:
				return static_cast<Result>( 1.0e+97 );
			case 98:
				return static_cast<Result>( 1.0e+98 );
			case 99:
				return static_cast<Result>( 1.0e+99 );
			case 100:
				return static_cast<Result>( 1.0e+100 );
			case 101:
				return static_cast<Result>( 1.0e+101 );
			case 102:
				return static_cast<Result>( 1.0e+102 );
			case 103:
				return static_cast<Result>( 1.0e+103 );
			case 104:
				return static_cast<Result>( 1.0e+104 );
			case 105:
				return static_cast<Result>( 1.0e+105 );
			case 106:
				return static_cast<Result>( 1.0e+106 );
			case 107:
				return static_cast<Result>( 1.0e+107 );
			case 108:
				return static_cast<Result>( 1.0e+108 );
			case 109:
				return static_cast<Result>( 1.0e+109 );
			case 110:
				return static_cast<Result>( 1.0e+110 );
			case 111:
				return static_cast<Result>( 1.0e+111 );
			case 112:
				return static_cast<Result>( 1.0e+112 );
			case 113:
				return static_cast<Result>( 1.0e+113 );
			case 114:
				return static_cast<Result>( 1.0e+114 );
			case 115:
				return static_cast<Result>( 1.0e+115 );
			case 116:
				return static_cast<Result>( 1.0e+116 );
			case 117:
				return static_cast<Result>( 1.0e+117 );
			case 118:
				return static_cast<Result>( 1.0e+118 );
			case 119:
				return static_cast<Result>( 1.0e+119 );
			case 120:
				return static_cast<Result>( 1.0e+120 );
			case 121:
				return static_cast<Result>( 1.0e+121 );
			case 122:
				return static_cast<Result>( 1.0e+122 );
			case 123:
				return static_cast<Result>( 1.0e+123 );
			case 124:
				return static_cast<Result>( 1.0e+124 );
			case 125:
				return static_cast<Result>( 1.0e+125 );
			case 126:
				return static_cast<Result>( 1.0e+126 );
			case 127:
				return static_cast<Result>( 1.0e+127 );
			case 128:
				return static_cast<Result>( 1.0e+128 );
			case 129:
				return static_cast<Result>( 1.0e+129 );
			case 130:
				return static_cast<Result>( 1.0e+130 );
			case 131:
				return static_cast<Result>( 1.0e+131 );
			case 132:
				return static_cast<Result>( 1.0e+132 );
			case 133:
				return static_cast<Result>( 1.0e+133 );
			case 134:
				return static_cast<Result>( 1.0e+134 );
			case 135:
				return static_cast<Result>( 1.0e+135 );
			case 136:
				return static_cast<Result>( 1.0e+136 );
			case 137:
				return static_cast<Result>( 1.0e+137 );
			case 138:
				return static_cast<Result>( 1.0e+138 );
			case 139:
				return static_cast<Result>( 1.0e+139 );
			case 140:
				return static_cast<Result>( 1.0e+140 );
			case 141:
				return static_cast<Result>( 1.0e+141 );
			case 142:
				return static_cast<Result>( 1.0e+142 );
			case 143:
				return static_cast<Result>( 1.0e+143 );
			case 144:
				return static_cast<Result>( 1.0e+144 );
			case 145:
				return static_cast<Result>( 1.0e+145 );
			case 146:
				return static_cast<Result>( 1.0e+146 );
			case 147:
				return static_cast<Result>( 1.0e+147 );
			case 148:
				return static_cast<Result>( 1.0e+148 );
			case 149:
				return static_cast<Result>( 1.0e+149 );
			case 150:
				return static_cast<Result>( 1.0e+150 );
			case 151:
				return static_cast<Result>( 1.0e+151 );
			case 152:
				return static_cast<Result>( 1.0e+152 );
			case 153:
				return static_cast<Result>( 1.0e+153 );
			case 154:
				return static_cast<Result>( 1.0e+154 );
			case 155:
				return static_cast<Result>( 1.0e+155 );
			case 156:
				return static_cast<Result>( 1.0e+156 );
			case 157:
				return static_cast<Result>( 1.0e+157 );
			case 158:
				return static_cast<Result>( 1.0e+158 );
			case 159:
				return static_cast<Result>( 1.0e+159 );
			case 160:
				return static_cast<Result>( 1.0e+160 );
			case 161:
				return static_cast<Result>( 1.0e+161 );
			case 162:
				return static_cast<Result>( 1.0e+162 );
			case 163:
				return static_cast<Result>( 1.0e+163 );
			case 164:
				return static_cast<Result>( 1.0e+164 );
			case 165:
				return static_cast<Result>( 1.0e+165 );
			case 166:
				return static_cast<Result>( 1.0e+166 );
			case 167:
				return static_cast<Result>( 1.0e+167 );
			case 168:
				return static_cast<Result>( 1.0e+168 );
			case 169:
				return static_cast<Result>( 1.0e+169 );
			case 170:
				return static_cast<Result>( 1.0e+170 );
			case 171:
				return static_cast<Result>( 1.0e+171 );
			case 172:
				return static_cast<Result>( 1.0e+172 );
			case 173:
				return static_cast<Result>( 1.0e+173 );
			case 174:
				return static_cast<Result>( 1.0e+174 );
			case 175:
				return static_cast<Result>( 1.0e+175 );
			case 176:
				return static_cast<Result>( 1.0e+176 );
			case 177:
				return static_cast<Result>( 1.0e+177 );
			case 178:
				return static_cast<Result>( 1.0e+178 );
			case 179:
				return static_cast<Result>( 1.0e+179 );
			case 180:
				return static_cast<Result>( 1.0e+180 );
			case 181:
				return static_cast<Result>( 1.0e+181 );
			case 182:
				return static_cast<Result>( 1.0e+182 );
			case 183:
				return static_cast<Result>( 1.0e+183 );
			case 184:
				return static_cast<Result>( 1.0e+184 );
			case 185:
				return static_cast<Result>( 1.0e+185 );
			case 186:
				return static_cast<Result>( 1.0e+186 );
			case 187:
				return static_cast<Result>( 1.0e+187 );
			case 188:
				return static_cast<Result>( 1.0e+188 );
			case 189:
				return static_cast<Result>( 1.0e+189 );
			case 190:
				return static_cast<Result>( 1.0e+190 );
			case 191:
				return static_cast<Result>( 1.0e+191 );
			case 192:
				return static_cast<Result>( 1.0e+192 );
			case 193:
				return static_cast<Result>( 1.0e+193 );
			case 194:
				return static_cast<Result>( 1.0e+194 );
			case 195:
				return static_cast<Result>( 1.0e+195 );
			case 196:
				return static_cast<Result>( 1.0e+196 );
			case 197:
				return static_cast<Result>( 1.0e+197 );
			case 198:
				return static_cast<Result>( 1.0e+198 );
			case 199:
				return static_cast<Result>( 1.0e+199 );
			case 200:
				return static_cast<Result>( 1.0e+200 );
			case 201:
				return static_cast<Result>( 1.0e+201 );
			case 202:
				return static_cast<Result>( 1.0e+202 );
			case 203:
				return static_cast<Result>( 1.0e+203 );
			case 204:
				return static_cast<Result>( 1.0e+204 );
			case 205:
				return static_cast<Result>( 1.0e+205 );
			case 206:
				return static_cast<Result>( 1.0e+206 );
			case 207:
				return static_cast<Result>( 1.0e+207 );
			case 208:
				return static_cast<Result>( 1.0e+208 );
			case 209:
				return static_cast<Result>( 1.0e+209 );
			case 210:
				return static_cast<Result>( 1.0e+210 );
			case 211:
				return static_cast<Result>( 1.0e+211 );
			case 212:
				return static_cast<Result>( 1.0e+212 );
			case 213:
				return static_cast<Result>( 1.0e+213 );
			case 214:
				return static_cast<Result>( 1.0e+214 );
			case 215:
				return static_cast<Result>( 1.0e+215 );
			case 216:
				return static_cast<Result>( 1.0e+216 );
			case 217:
				return static_cast<Result>( 1.0e+217 );
			case 218:
				return static_cast<Result>( 1.0e+218 );
			case 219:
				return static_cast<Result>( 1.0e+219 );
			case 220:
				return static_cast<Result>( 1.0e+220 );
			case 221:
				return static_cast<Result>( 1.0e+221 );
			case 222:
				return static_cast<Result>( 1.0e+222 );
			case 223:
				return static_cast<Result>( 1.0e+223 );
			case 224:
				return static_cast<Result>( 1.0e+224 );
			case 225:
				return static_cast<Result>( 1.0e+225 );
			case 226:
				return static_cast<Result>( 1.0e+226 );
			case 227:
				return static_cast<Result>( 1.0e+227 );
			case 228:
				return static_cast<Result>( 1.0e+228 );
			case 229:
				return static_cast<Result>( 1.0e+229 );
			case 230:
				return static_cast<Result>( 1.0e+230 );
			case 231:
				return static_cast<Result>( 1.0e+231 );
			case 232:
				return static_cast<Result>( 1.0e+232 );
			case 233:
				return static_cast<Result>( 1.0e+233 );
			case 234:
				return static_cast<Result>( 1.0e+234 );
			case 235:
				return static_cast<Result>( 1.0e+235 );
			case 236:
				return static_cast<Result>( 1.0e+236 );
			case 237:
				return static_cast<Result>( 1.0e+237 );
			case 238:
				return static_cast<Result>( 1.0e+238 );
			case 239:
				return static_cast<Result>( 1.0e+239 );
			case 240:
				return static_cast<Result>( 1.0e+240 );
			case 241:
				return static_cast<Result>( 1.0e+241 );
			case 242:
				return static_cast<Result>( 1.0e+242 );
			case 243:
				return static_cast<Result>( 1.0e+243 );
			case 244:
				return static_cast<Result>( 1.0e+244 );
			case 245:
				return static_cast<Result>( 1.0e+245 );
			case 246:
				return static_cast<Result>( 1.0e+246 );
			case 247:
				return static_cast<Result>( 1.0e+247 );
			case 248:
				return static_cast<Result>( 1.0e+248 );
			case 249:
				return static_cast<Result>( 1.0e+249 );
			case 250:
				return static_cast<Result>( 1.0e+250 );
			case 251:
				return static_cast<Result>( 1.0e+251 );
			case 252:
				return static_cast<Result>( 1.0e+252 );
			case 253:
				return static_cast<Result>( 1.0e+253 );
			case 254:
				return static_cast<Result>( 1.0e+254 );
			case 255:
				return static_cast<Result>( 1.0e+255 );
			case 256:
				return static_cast<Result>( 1.0e+256 );
			case 257:
				return static_cast<Result>( 1.0e+257 );
			case 258:
				return static_cast<Result>( 1.0e+258 );
			case 259:
				return static_cast<Result>( 1.0e+259 );
			case 260:
				return static_cast<Result>( 1.0e+260 );
			case 261:
				return static_cast<Result>( 1.0e+261 );
			case 262:
				return static_cast<Result>( 1.0e+262 );
			case 263:
				return static_cast<Result>( 1.0e+263 );
			case 264:
				return static_cast<Result>( 1.0e+264 );
			case 265:
				return static_cast<Result>( 1.0e+265 );
			case 266:
				return static_cast<Result>( 1.0e+266 );
			case 267:
				return static_cast<Result>( 1.0e+267 );
			case 268:
				return static_cast<Result>( 1.0e+268 );
			case 269:
				return static_cast<Result>( 1.0e+269 );
			case 270:
				return static_cast<Result>( 1.0e+270 );
			case 271:
				return static_cast<Result>( 1.0e+271 );
			case 272:
				return static_cast<Result>( 1.0e+272 );
			case 273:
				return static_cast<Result>( 1.0e+273 );
			case 274:
				return static_cast<Result>( 1.0e+274 );
			case 275:
				return static_cast<Result>( 1.0e+275 );
			case 276:
				return static_cast<Result>( 1.0e+276 );
			case 277:
				return static_cast<Result>( 1.0e+277 );
			case 278:
				return static_cast<Result>( 1.0e+278 );
			case 279:
				return static_cast<Result>( 1.0e+279 );
			case 280:
				return static_cast<Result>( 1.0e+280 );
			case 281:
				return static_cast<Result>( 1.0e+281 );
			case 282:
				return static_cast<Result>( 1.0e+282 );
			case 283:
				return static_cast<Result>( 1.0e+283 );
			case 284:
				return static_cast<Result>( 1.0e+284 );
			case 285:
				return static_cast<Result>( 1.0e+285 );
			case 286:
				return static_cast<Result>( 1.0e+286 );
			case 287:
				return static_cast<Result>( 1.0e+287 );
			case 288:
				return static_cast<Result>( 1.0e+288 );
			case 289:
				return static_cast<Result>( 1.0e+289 );
			case 290:
				return static_cast<Result>( 1.0e+290 );
			case 291:
				return static_cast<Result>( 1.0e+291 );
			case 292:
				return static_cast<Result>( 1.0e+292 );
			case 293:
				return static_cast<Result>( 1.0e+293 );
			case 294:
				return static_cast<Result>( 1.0e+294 );
			case 295:
				return static_cast<Result>( 1.0e+295 );
			case 296:
				return static_cast<Result>( 1.0e+296 );
			case 297:
				return static_cast<Result>( 1.0e+297 );
			case 298:
				return static_cast<Result>( 1.0e+298 );
			case 299:
				return static_cast<Result>( 1.0e+299 );
			case 300:
				return static_cast<Result>( 1.0e+300 );
			case 301:
				return static_cast<Result>( 1.0e+301 );
			case 302:
				return static_cast<Result>( 1.0e+302 );
			case 303:
				return static_cast<Result>( 1.0e+303 );
			case 304:
				return static_cast<Result>( 1.0e+304 );
			case 305:
				return static_cast<Result>( 1.0e+305 );
			case 306:
				return static_cast<Result>( 1.0e+306 );
			case 307:
				return static_cast<Result>( 1.0e+307 );
			case 308:
				return static_cast<Result>( 1.0e+308 );
			}
			n -= 308;
			auto result = static_cast<Result>( 1.0e+308 );
			while( n > 1 ) {
				result *= static_cast<Result>( 10 );
				--n;
			}
			return result;
		}

		template<typename Number, std::enable_if_t<daw::is_arithmetic_v<Number>,
		                                           std::nullptr_t> = nullptr>
		constexpr int16_t whole_log10( Number positive_value ) noexcept {
			for( int16_t n = std::numeric_limits<Number>::max_exponent10; n > 0;
			     --n ) {
				if( positive_value >= pow10<Number>( n ) ) {
					return n;
				}
			}
			return 0;
		}

		template<typename F>
		constexpr bool is_inf( F value ) noexcept {
			return value > std::numeric_limits<F>::max( );
		}
		template<typename Float>
		constexpr bool is_nan( Float value ) noexcept {
			return value != value;
		}

		struct unexpected_state {};

		template<typename Float>
		constexpr static int max_fractional_digits =
		  std::numeric_limits<Float>::min_exponent10 * -1;
	} // namespace impl

	// character pointer
	template<typename CharT, std::enable_if_t<daw::traits::is_character_v<CharT>,
	                                          std::nullptr_t> = nullptr>
	constexpr daw::basic_string_view<CharT>
	to_string( CharT const *str ) noexcept {
		return daw::basic_string_view<CharT>( str );
	}

	// string_view
	template<typename CharT>
	constexpr daw::basic_string_view<CharT>
	to_string( daw::basic_string_view<CharT> str ) noexcept {
		return str;
	}

	// std::basic_string
	// TODO add overload for the operator<< to use accepts_asciiz
	template<typename CharT>
	daw::basic_string_view<CharT>
	to_string( std::basic_string<CharT> const &str ) noexcept {
		return daw::basic_string_view<CharT>( str.data( ), str.size( ) );
	}

	// Integer numbers
	template<
	  typename CharT, typename Integer,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_integral_v<daw::remove_cvref_t<Integer>>,
	                    !daw::is_same_v<bool, daw::remove_cvref_t<Integer>>,
	                    !daw::is_floating_point_v<daw::remove_cvref_t<Integer>>,
	                    !daw::traits::is_character_v<Integer>>,
	    std::nullptr_t> = nullptr>
	static constexpr auto to_string( Integer value ) {
		size_t const buff_size = std::numeric_limits<Integer>::digits10;
		daw::static_string_t<CharT, buff_size> result{};

		if( value < 0 ) {
			result += daw::char_traits<CharT>::minus;
			value *= -1;
		}
		for( auto pow10 = impl::pow10<Integer>( impl::whole_log10( value ) );
		     pow10 >= 1; pow10 /= 10 ) {

			auto const tmp = value / pow10;

			daw::exception::dbg_precondition_check(
			  tmp >= 0 && tmp < 10,
			  "There should only ever be a single digit positive number" );

			result += daw::char_traits<CharT>::get_char_digit( tmp );

			value -= tmp * 10;
		}
		return result;
	}

	// Bool
	template<typename CharT, typename Bool,
	         std::enable_if_t<
	           daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                           daw::is_same_v<char, daw::remove_cvref_t<CharT>>>,
	           std::nullptr_t> = nullptr>
	constexpr auto to_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( "true" );
		}
		return daw::static_string_t<CharT, 5>( "false" );
	}

	template<
	  typename CharT, typename Bool,
	  std::enable_if_t<
	    daw::all_true_v<daw::is_same_v<bool, daw::remove_cvref_t<Bool>>,
	                    daw::is_same_v<wchar_t, daw::remove_cvref_t<CharT>>>,
	    std::nullptr_t> = nullptr>
	constexpr auto to_string( Bool b ) noexcept {
		if( b ) {
			return daw::static_string_t<CharT, 5>( L"true" );
		}
		return daw::static_string_t<CharT, 5>( L"false" );
	}

	// Single character (char, wchar_t).  Need to be treated separately from other
	// integers
	template<typename CharT,
	         std::enable_if_t<::daw::traits::is_character_v<CharT>,
	                          std::nullptr_t> = nullptr>
	constexpr auto to_string( CharT c ) noexcept {
		daw::static_string_t<CharT, 1> result{};
		result.push_back( c );
		return result;
	}

	// Floating point numbers
	template<
	  typename CharT, typename Float,
	  std::enable_if_t<daw::is_floating_point_v<Float>, std::nullptr_t> = nullptr>
	constexpr auto
	to_string( Float value,
	           int significant_digits = std::numeric_limits<Float>::max_digits10,
	           int precision = std::numeric_limits<Float>::max_digits10 ) {

		size_t const buff_size = std::numeric_limits<Float>::max_exponent10 + 2;
		daw::static_string_t<CharT, buff_size> result{};

		if( value == static_cast<Float>( 0 ) ||
		    value == static_cast<Float>( -0.0 ) ) {
			result += daw::char_traits<CharT>::get_char_digit( 0 );
			return result;
		}
		if( impl::is_nan( value ) ) {
			result += daw::char_traits<CharT>::nan( );
			return result;
		}
		if( value < 0 ) {
			result += daw::char_traits<CharT>::minus;
			value = -value;
		}
		if( impl::is_inf( value ) ) {
			result += daw::char_traits<CharT>::inf( );
			return result;
		}

		// Whole numbers
		auto const e = impl::whole_log10( value );
		auto tmp_value = value;

		auto const max_dig = static_cast<int16_t>( daw::max(
		  0, static_cast<int>( e ) -
		       static_cast<int>( daw::min( std::numeric_limits<Float>::max_digits10,
		                                   significant_digits ) ) ) );

		for( int16_t ex = e; ex >= max_dig; --ex ) {
			auto const p10 = impl::pow10<Float>( ex );
			auto const digit = static_cast<char>( tmp_value / p10 );

			daw::exception::dbg_precondition_check<impl::unexpected_state>(
			  digit >= 0 && digit <= 10 );

			result += daw::char_traits<CharT>::get_char_digit( digit );

			tmp_value -= static_cast<Float>( digit ) * p10;
		}
		for( int16_t ex = max_dig; ex > 0; --ex ) {
			result += daw::char_traits<CharT>::get_char_digit( 0 );
		}
		if( e >= std::numeric_limits<Float>::max_digits10 ||
		    e >= significant_digits ||
		    tmp_value <= std::numeric_limits<Float>::min( ) ) {
			return result;
		}

		// Fractional Part
		result += daw::char_traits<CharT>::decimal_point;

		auto const num_dec_digits =
		  daw::min( daw::min( precision, impl::max_fractional_digits<Float> ),
		            std::numeric_limits<Float>::max_digits10 - e );

		for( int n = 0;
		     n < num_dec_digits && tmp_value > std::numeric_limits<Float>::min( );
		     ++n ) {
			auto const digit =
			  static_cast<char>( tmp_value * static_cast<Float>( 10 ) );

			result += daw::char_traits<CharT>::get_char_digit( digit );
			tmp_value -= static_cast<Float>( digit ) / static_cast<Float>( 10 );
			tmp_value *= static_cast<Float>( 10 );
		}
		while( result[result.size( ) - 2] !=
		         daw::char_traits<CharT>::decimal_point &&
		       result.back( ) == daw::char_traits<CharT>::zero ) {
			result.resize( result.size( ) - 1, false );
		}
		return result;
	} // namespace ostream_converters

	namespace impl {
		template<typename T>
		using has_tostring_detect2 =
		  decltype( to_string( std::declval<T const &>( ) ) );

		namespace to_string_detect {
			using ::ostream_converters::to_string;
			template<typename CharT, typename T>
			using has_to_string_detect =
			  decltype( to_string<CharT>( std::declval<T const &>( ) ) );
		} // namespace to_string_detect

	} // namespace impl

	template<typename CharT, typename T>
	constexpr bool has_to_string_v =
	  daw::is_detected_v<impl::to_string_detect::has_to_string_detect, CharT,
	                     ::daw::remove_cvref_t<T>>;
} // namespace ostream_converters
