//
// The Eternity Engine
// Copyright(C) 2018 James Haley, Max Waine, et al.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/
//
// Additional terms and conditions compatible with the GPLv3 apply. See the
// file COPYING-EE for details.
//
// Purpose: Aeon wrapper for angle_t
// Authors: Max Waine
//

#include "aeon_angle.h"
#include "aeon_common.h"
#include "aeon_fixed.h"
#include "aeon_system.h"
#include "c_io.h"
#include "m_fixed.h"
#include "tables.h"

//
// Clamps a fixed_t to be within the range representable by an angle_t
// then converts it to an angle_t
//
static inline angle_t FixedToAngleClamped(const fixed_t val)
{
   return FixedToAngle(val % (360 << FRACBITS));
}

AeonAngle AeonAngle::operator + (const AeonAngle &in)  { return value + in.value; }
AeonAngle AeonAngle::operator + (const AeonFixed &in)  { return value + (FixedToAngle(in.value)); }
AeonAngle AeonAngle::operator + (const int val)        { return value + (val * ANGLE_1); }

AeonAngle AeonAngle::operator * (const AeonFixed &in)
{
   return FixedToAngleClamped(FixedMul(AngleToFixed(value), in.value));
}
AeonAngle AeonAngle::operator * (const int val)        { return value * val; }

AeonAngle AeonAngle::operator / (const AeonAngle &in)  { return value / in.value; }
AeonAngle AeonAngle::operator / (const AeonFixed &in)
{
   return FixedToAngleClamped(FixedDiv(AngleToFixed(value), in.value));
}
AeonAngle AeonAngle::operator / (const int val) { return value / val; }

AeonAngle &AeonAngle::operator += (const AeonAngle &in)
{
   value += in.value;
   return *this;
}
AeonAngle &AeonAngle::operator += (const AeonFixed &in)
{
   value += FixedToAngleClamped(in.value);
   return *this;
}
AeonAngle &AeonAngle::operator += (const int val)
{
   value += val * ANGLE_1;
   return *this;
}

AeonAngle &AeonAngle::operator *= (const AeonFixed &in)
{
   value = FixedToAngleClamped(FixedMul(AngleToFixed(value), in.value));
   return *this;
}
AeonAngle &AeonAngle::operator *= (const int val)
{
   value *= val;
   return *this;
}

AeonAngle &AeonAngle::operator /= (const AeonAngle &in)
{
   value /= in.value;
   return *this;
}
AeonAngle &AeonAngle::operator /= (const AeonFixed &in)
{
   value = FixedToAngleClamped(FixedDiv(AngleToFixed(value), in.value));
   return *this;
}
AeonAngle &AeonAngle::operator /= (const int val)
{
   value /= val;
   return *this;
}

AeonAngle::operator fixed_t() const { return AeonFixed(AngleToFixed(value)); }

#define ANGLEBINOP(op, param) \
   WRAP_MFN_PR(AeonAngle, operator op,  (param), AeonAngle)

#define ANGLEASSIGNOP(op, param) \
   WRAP_MFN_PR(AeonAngle, operator op,  (param), AeonAngle &)

void AeonScriptObjAngle::Construct(AeonAngle *thisFixed)
{
   *thisFixed = AeonAngle();
}

void AeonScriptObjAngle::ConstructFromOther(const AeonAngle &other, AeonAngle *thisAngle)
{
   *thisAngle = AeonAngle(other);
}

void AeonScriptObjAngle::ConstructFromDouble(double other, AeonAngle *thisAngle)
{
   thisAngle->value = FixedToAngleClamped(M_DoubleToFixed(other));
}

void AeonScriptObjAngle::ConstructFromInt(int other, AeonAngle *thisAngle)
{
   thisAngle->value = other * ANGLE_1;
}

static void ASPrint(AeonAngle f)
{
   C_Printf("%f\n", M_FixedToDouble(AngleToFixed(f.value)));
}

static aeonfuncreg_t angleFuncs[] =
{
   { "angle opAdd(const angle &in)",        ANGLEBINOP(+, const AeonAngle &)         },
   { "angle opAdd(const fixed &in)",        ANGLEBINOP(+, const AeonFixed &)         },
   { "angle opAdd(const int val)",          ANGLEBINOP(+, const int),                },
   { "angle opMul(const fixed &in)",        ANGLEBINOP(*, const AeonFixed &)         },
   { "angle opMul(const int val)",          ANGLEBINOP(*, const int)                 },
   { "angle opDiv(const angle &in)",        ANGLEBINOP(/, const AeonAngle &)         },
   { "angle opDiv(const fixed &in)",        ANGLEBINOP(/, const AeonFixed &)         },
   { "angle opDiv(const int val)",          ANGLEBINOP(/, const int)                 },
   { "angle &opAddAssign(const angle &in)", ANGLEASSIGNOP(+=, const AeonAngle &)     },
   { "angle &opAddAssign(const fixed &in)", ANGLEASSIGNOP(+=, const AeonFixed &)     },
   { "angle &opAddAssign(const int val)",   ANGLEASSIGNOP(+=, const int)             },
   { "angle &opMulAssign(const angle &in)", ANGLEASSIGNOP(*=, const AeonFixed &)     },
   { "angle &opMulAssign(const int val)",   ANGLEASSIGNOP(*=, const int)             },
   { "angle &opDivAssign(const angle &in)", ANGLEASSIGNOP(/=, const AeonAngle &)     },
   { "angle &opDivAssign(const fixed &in)", ANGLEASSIGNOP(/=, const AeonFixed &)     },
   { "angle &opDivAssign(const int val)",   ANGLEASSIGNOP(/=, const int)             },
   { "fixed opImplConv() const",            WRAP_MFN(AeonAngle, operator fixed_t)    },
};

void AeonScriptObjAngle::Init()
{
   asIScriptEngine *e = AeonScriptManager::Engine();

   e->RegisterObjectType("angle", sizeof(AeonAngle),
                         asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA | asOBJ_APP_CLASS_ALLINTS);

   e->RegisterObjectBehaviour("angle", asBEHAVE_CONSTRUCT, "void f()",
                              WRAP_OBJ_LAST(Construct), asCALL_GENERIC);
   e->RegisterObjectBehaviour("angle", asBEHAVE_CONSTRUCT, "void f(const angle &in)",
                              WRAP_OBJ_LAST(ConstructFromOther), asCALL_GENERIC);
   e->RegisterObjectBehaviour("angle", asBEHAVE_CONSTRUCT, "void f(const double)",
                              WRAP_OBJ_LAST(ConstructFromDouble), asCALL_GENERIC);
   e->RegisterObjectBehaviour("angle", asBEHAVE_CONSTRUCT, "void f(const int)",
                              WRAP_OBJ_LAST(ConstructFromInt), asCALL_GENERIC);

   for(const aeonfuncreg_t &fn : angleFuncs)
      e->RegisterObjectMethod("angle", fn.declaration, fn.funcPointer, asCALL_GENERIC);

   e->RegisterGlobalFunction("void print(fixed)", WRAP_FN_PR(ASPrint, (AeonAngle), void),
                             asCALL_GENERIC);
}

// EOF

