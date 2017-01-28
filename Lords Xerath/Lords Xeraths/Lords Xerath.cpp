
#include "PluginSDK.h"


// Lord's Xerath.cpp : Defines the exported functions for the DLL application.
//



PluginSetup("Lord's Xerath - LordZEDith");

IMenu* MainMenu;
IMenu* QMenu;
IMenu* WMenu;
IMenu* EMenu;
IMenu* RMenu;
IMenu* Drawings;
IMenuOption* ComboQ;
IMenuOption* FarmQ;
IMenuOption* ComboW;
IMenuOption* FarmW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* ComboR2;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* HealthPercent;

ISpell2* Q = nullptr;
ISpell2* W;
ISpell2* E;
ISpell2* R;

bool isPressingKeyToShootR = false;

void  Menu()
{
	MainMenu = GPluginSDK->AddMenu("Lords Xerath");
	QMenu = MainMenu->AddMenu("Q Settings");
	WMenu = MainMenu->AddMenu("W Settings");
	EMenu = MainMenu->AddMenu("E Settings");
	RMenu = MainMenu->AddMenu("R Settings");
	Drawings = MainMenu->AddMenu("Drawings");

	ComboQ = QMenu->CheckBox("Use Q", true);
	//FarmQ = QMenu->CheckBox("Use Q Farm", true);

	ComboW = WMenu->CheckBox("Use W", true);
	//FarmW = WMenu->CheckBox("Use W Farm", true);

	ComboE = EMenu->CheckBox("Use E", true);
 
	ComboR = RMenu->CheckBox("Use R(When u select it)", true);
	//ComboR2 = RMenu->CheckBox("Auto R when Enemy Killable", true);
	//HealthPercent = RMenu->AddInteger("Enemies Health %", 0, 100, 40);
	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);

	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", true);
	DrawE = Drawings->CheckBox("Draw E", true);
}
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, kCollidesWithNothing);
	Q->SetCharged(750.f, 1550.f, 1.45f); // Min range: 750 | Max range: 1550 | Total charge time: 3.0 seconds
	W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithNothing));
	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall));
	R = GPluginSDK->CreateSpell2(kSlotR, kCircleCast, true, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
	Q->SetOverrideRange(1550);
	W->SetOverrideRange(1100);
	E->SetOverrideRange(1050);
	//R->SetOverrideRange(675);
    Q->SetOverrideDelay(0.6f);
	W->SetOverrideDelay(0.7f);
	E->SetOverrideDelay(0.25f);
	//R->SetOverrideDelay(0.7f);
	Q->SetOverrideRadius(95);
	W->SetOverrideRadius(125);
	E->SetOverrideRadius(60);
	//R->SetOverrideRadius(130);
	E->SetOverrideSpeed(1400);
}
int GetEnemiesInRange(float range)
{
	auto enemies = GEntityList->GetAllHeros(false, true);
	auto enemiesInRange = 0;

	for (auto enemy : enemies)
	{
		if (enemy != nullptr && enemy->GetTeam() != GEntityList->Player()->GetTeam())
		{
			auto flDistance = (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length();
			if (flDistance < range)
			{
				enemiesInRange++;
			}
		}
	}
	return enemiesInRange;
}
void Combo()
{
	if (ComboQ->Enabled())
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	
			
			if (GetEnemiesInRange(1550) >= 1)
			{
				Q->StartCharging();
			}

			if (Q->IsCharging())
			{
				target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
				//if (target != nullptr)
			
					Q->FindTarget(SpellDamage);
					Q->CastOnTarget(target, 5);
			
					
			}	

		}
	}
	if (ComboW->Enabled())
	{
		if (W->IsReady() && W->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
		//	if (target != nullptr)
		
				W->CastOnTarget(target, 6);
		
		}
	}
	if (ComboE->Enabled())
	{
		if (E->IsReady() && E->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
		//	if (target != nullptr)
		
				E->CastOnTarget(target, 6);
		
		}
	}
	/*if (ComboR->Enabled() && R->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
		if (target->HealthPercent() <= HealthPercent->GetInteger())
		{
			if (target != nullptr)
			{
				R->CastOnTarget(target, kHitChanceHigh);
			}
		}
	}*/
	
}
bool CastingR()
{
	if (GEntityList->Player()->HasBuff("XerathLocusOfPower2") || GEntityList->Player()->HasBuff("XerathLocusPulse"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void RunAlways()
{
	if (R->IsReady() && ComboR->Enabled())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
		if (CastingR() == true)
		{
			R->CastOnTarget(target, 5);

		}
	}
}
/*void Automatic(IUnit* Local)
{
	if (ComboR2->Enabled())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (hero == nullptr || hero->IsDead())
				continue;

			if (R->IsReady() && Local->IsValidTarget(hero, R->Range()))
			{
				auto dmg = GHealthPrediction->GetKSDamage(hero, kSlotW, R->GetDelay(), true);

				if (dmg > hero->GetHealth())
				{
					if (R->CastOnTarget(hero, kHitChanceHigh))
						return;
				}
			}
		}
	}
}*/
/*PLUGIN_EVENT(bool) OnWndProc(HWND Wnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
#if defined PIPEX_DEBUG && defined KEY_PRESSES_STUFF
	if (Message == WM_CHAR)
	{
		char message[64];
		sprintf(message, "Key pressed: %i", wParam);
		GGame->PrintChat(message);
	}
#endif

	if (Message == WM_CHAR && wParam == ComboR->GetInteger())
	{
		isPressingKeyToShootR = true;
		return false;
	}
	if (Message = WM_KEYUP && wParam == ComboR->GetInteger())
	{
		isPressingKeyToShootR = false;
		return false;
	}

	return true;
}*/
 


	 


	 
	PLUGIN_EVENT(void) OnRender()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
		}
	}
	PLUGIN_EVENT(void) OnGameUpdate()
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Combo();
		}
		RunAlways();
	//	if (GOrbwalking->GetOrbwalkingMode()== kModeMixed)
	//	{
		//	if (isPressingKeyToShootR && GEntityList->Player()->GetSpellState(kSlotR) == Ready)
		//	{
		//		IUnit *rTarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
		//		if (rTarget)
		//		{
		//			R->CastOnTarget(rTarget, 5);
		//		}
	//		}
	//	}

		
	}
	PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
	{

		PluginSDKSetup(PluginSDK);
		Menu();
		LoadSpells();


		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnRender, OnRender);

	}

	PLUGIN_API void OnUnload()
	{
		MainMenu->Remove();

	
		GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->RemoveEventHandler(kEventOnRender, OnRender);

	}

	