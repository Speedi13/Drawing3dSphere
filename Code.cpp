struct SphereAngles
{
	float AngleSin;
	float AngleCos;
};

void Draw3DCircleForSphere(  float X, float Y, float Z, float flRadius, int numSides, D3DCOLOR color, D3DXMATRIX* rot, SphereAngles* AnglesArray, Vec3* OutCirclePositions, bool bVertical = false )
{
	Vec4 m_vecEnd;
	
	bool bSkip = false;

	X -= rot->m[3][0];
	Y -= rot->m[3][1];
	Z -= rot->m[3][2];

	if ( !(flRadius > 0) )
	{
		Vec4 WorldPos = Vec4( X, Y, Z, 0 );
		Vec3Transform( &WorldPos, (Vec3*)&WorldPos, rot );
		Vec3 ScreenPos;
		bool bOnScreenVisible = ESP::GetInstance()->ScreenProject( *(Vec3*)&WorldPos, &ScreenPos );
		for (int i = 0; i < numSides; i++)
		{
			if ( bOnScreenVisible )
			{
				OutCirclePositions[i] = ScreenPos;
				OutCirclePositions[i].z = 1;
			}
			else
				OutCirclePositions[i].z = 0;
		}
		return;
	}

	//float flStep = (float)(D3DX_PI*2) / ((float)(numSides-1));

	for (int i = 0, i_ = 0; i < (numSides+1); i++)
	{
		i_ = i;
		if ( i_ >= numSides )
			i_ = 0;

		//float angle =  ( flStep * (float)i_ ); //= D3DXToRadian( 360.0f / ((float)numSides) * (float)i_ );

		Vec4
			m_vecStart = Vec4(
				(float)X,
				(float)Y + flRadius * (float)AnglesArray[i_].AngleSin ,//sin(angle),
				(float)Z,
				0.0f
		);

		if ( !bVertical )
			m_vecStart.x += flRadius * (float)AnglesArray[i_].AngleCos ;//(float)cos(angle);
		else
			m_vecStart.z += flRadius * (float)AnglesArray[i_].AngleCos ;//(float)cos(angle);

		Vec3Transform( &m_vecStart, (Vec3*)&m_vecStart, rot );
		if (i != 0)
			*(BYTE*)&OutCirclePositions[i_].z = 0;

		if ( ESP::GetInstance()->ScreenProject( *(Vec3*)&m_vecStart, (Vec3*)&m_vecStart ) )
		{
			if (i != 0)
			{
				OutCirclePositions[i_] = *(Vec3*)&m_vecStart;
				*(BYTE*)&OutCirclePositions[i_].z = 1; //for optimisation, I don't want to use float comparison
				if ( bSkip != true )
					render->DrawLine( m_vecStart.x, m_vecStart.y, m_vecEnd.x, m_vecEnd.y, color );
			}
			m_vecEnd = m_vecStart;
			bSkip = false;
		}
		else
			bSkip = true;
	}
}
void DrawSphere( Vec3 Center, D3DXMATRIX matrix, float Radius, int numSides, D3DCOLOR CircleColor )
{
	Vec3** CirclePositions = (Vec3**)alloca( (numSides + 1) * sizeof(DWORD_PTR) );

	float flStep = (float)(D3DX_PI) / ((float)(numSides-1));

	SphereAngles* AnglesArray = (SphereAngles*)alloca( (numSides + 1) * sizeof(SphereAngles) );
	for (int i = 0; i < numSides; i++)
	{
		float angle =  flStep * (float)i;
		AnglesArray[i].AngleSin = sin(angle);
		AnglesArray[i].AngleCos = cos(angle);
	}
	
	//same as using: float flStep2X = (float)(D3DX_PI*2) / ((float)(numSides-1));
	SphereAngles* AnglesArrayx2 = (SphereAngles*)alloca( (numSides + 1) * sizeof(SphereAngles) );
	for (int i = 0; i < numSides; i++)
	{
		int iPos = i*2;
		float flSign = 1;
		if ( iPos >= numSides )
		{
			iPos = (iPos - numSides) + 1;
			flSign = -1;
		}
		AnglesArrayx2[i].AngleSin = AnglesArray[iPos].AngleSin*flSign;
		AnglesArrayx2[i].AngleCos = AnglesArray[iPos].AngleCos*flSign;
	}

	for (int i = 0; i < numSides; i++)
	{
		CirclePositions[i] = (Vec3*)alloca( (numSides+1) * sizeof(Vec3) );

		//float angle = flStep * (float)i; //= D3DXToRadian( 360.0f / ((float)numSides) * (float)i );

		float flRadiusSin = (float)Radius * (float)AnglesArray[i].AngleSin;//sin(angle);//
		float flRadiusCos = (float)Radius * (float)AnglesArray[i].AngleCos;//cos(angle);//

		Draw3DCircleForSphere( Center.x, Center.y, Center.z + flRadiusCos, flRadiusSin, numSides, CircleColor, &matrix, AnglesArrayx2, CirclePositions[i], false );
		//Draw3DCircleForSphere( Center.x + flRadiusCos, Center.y, Center.z, flRadiusSin, numSides, CircleColor, &matrix, AnglesArrayx2, CirclePositions[i], true ); //<< using vertical 3D circles
	}

	for (int i = 0; i < numSides; i++)
	{
		for (int j = 0; j < numSides-1; j++)
		{
			Vec3 start = CirclePositions[j][i];
			if ( *(BYTE*)&start.z == 1 ) //for optimisation, I don't want to use float comparison
			{
				Vec3 end = CirclePositions[j+1][i] ;
				if ( *(BYTE*)&end.z == 1 )
				{
					render->DrawLine( start.x, start.y, end.x,end.y,CircleColor);
				}
			}
		}
	}
}
