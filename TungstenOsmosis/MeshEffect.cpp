#include "pch.h"
#include "MeshEffect.h"
#include "Tungsten\ServiceLocator.h"

using namespace TungstenOsmosis;
using namespace Tungsten;

TungstenOsmosis::MeshEffect::MeshEffect()
{
	_impl = new Tungsten::MeshEffect();
}

void TungstenOsmosis::MeshEffect::Draw()
{
	_impl->Draw();
}

void TungstenOsmosis::MeshEffect::Apply()
{
	_impl->Apply();
}
