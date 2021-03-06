/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::basicSourceList::correct
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    const word& fieldName = fld.name();

    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Correcting source " << source.name()
                        << " for field " << fieldName << endl;
                }

                source.correct(fld);
            }
        }
    }
}


template<class Type>
Foam::tmp<Foam::fvMatrix<Type> > Foam::basicSourceList::operator()
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(fld, fld.name());
}


template<class Type>
Foam::tmp<Foam::fvMatrix<Type> > Foam::basicSourceList::operator()
(
    GeometricField<Type, fvPatchField, volMesh>& fld,
    const word& fieldName
)
{
    checkApplied();

    const dimensionSet ds = fld.dimensions()/dimTime*dimVolume;

    tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(fld, ds));

    fvMatrix<Type>& mtx = tmtx();


    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type, class RhoType>
Foam::tmp<Foam::fvMatrix<Type> > Foam::basicSourceList::operator()
(
    const RhoType& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(rho, fld, fld.name());
}


template<class Type, class RhoType>
Foam::tmp<Foam::fvMatrix<Type> > Foam::basicSourceList::operator()
(
    const RhoType& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld,
    const word& fieldName
)
{
    checkApplied();

    const dimensionSet ds = rho.dimensions()*fld.dimensions()/dimTime*dimVolume;

    tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(fld, ds));

    fvMatrix<Type>& mtx = tmtx();


    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type>
void Foam::basicSourceList::constrain(fvMatrix<Type>& eqn)
{
    constrain(eqn, eqn.psi().name());
}


template<class Type>
void Foam::basicSourceList::constrain
(
    fvMatrix<Type>& eqn,
    const word& fieldName
)
{
    checkApplied();

    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying constraint " << source.name()
                        << " to field " << fieldName << endl;
                }

                source.setValue(eqn, fieldI);
            }
        }
    }
}


// ************************************************************************* //
