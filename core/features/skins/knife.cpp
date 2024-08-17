#include "../features.hpp"


bool apply_knife_model(attributable_item_t* weapon, const char* model, player_t* local) noexcept
{
	auto viewmodel = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(local->view_model()));
	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->weapon();
	if (!h_view_model_weapon)
		return false;

	auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(h_view_model_weapon));
	if (view_model_weapon != weapon)
		return false;

	viewmodel->model_index() = interfaces::model_info->get_model_index(model);

	return true;
}

void skins::run()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_ent = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_ent || !local_ent->is_alive())
		return;

	static const auto model_default_t = "models/weapons/v_knife_default_t.mdl";
	static const auto model_default_ct = "models / weapons / v_knife_default_ct.mdl";
	static const auto model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	static const auto model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	static const auto model_karambit = "models/weapons/v_knife_karam.mdl";
	static const auto model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	static const auto model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	static const auto model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	static const auto model_flip = "models/weapons/v_knife_flip.mdl";
	static const auto model_gut = "models/weapons/v_knife_gut.mdl";
	static const auto model_huntsman = "models/weapons/v_knife_tactical.mdl";
	static const auto model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	static const auto model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	static const auto model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	static const auto model_talon = "models/weapons/v_knife_widowmaker.mdl";
	static const auto model_ursus = "models/weapons/v_knife_ursus.mdl";
	static const auto model_nomad = "models/weapons/v_knife_outdoor.mdl";
	static const auto model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	static const auto model_survival = "models/weapons/v_knife_canis.mdl";
	static const auto model_paracord = "models/weapons/v_knife_cord.mdl";
	static const auto model_css = "models/weapons/v_knife_css.mdl";

	static const auto index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	static const auto index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	static const auto index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	static const auto index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	static const auto index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	static const auto index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	static const auto index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	static const auto index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	static const auto index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	static const auto index_shadow_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	static const auto index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	static const auto index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	static const auto index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	static const auto index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	static const auto index_nomad = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	static const auto index_skeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	static const auto index_survival = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	static const auto index_paracord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	static const auto index_css = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");

	auto active_weapon = local_ent->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_ent->get_weapons();
	for (size_t i = 0; my_weapons[i] != 0xFFFFFFFF; i++) {
		auto weapon_atr = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
		auto weapon = reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));

		if (!weapon_atr || !weapon || weapon->is_c4() || weapon->is_nade() || weapon->is_taser())
			return;

		if (weapon->is_knife()) {
			switch (variables::knife_model) {
			case 0:
				break;
			case 1:
				apply_knife_model(weapon_atr, model_bayonet, local_ent);
				break;
			case 2:
				apply_knife_model(weapon_atr, model_flip, local_ent);
				break;
			case 3:
				apply_knife_model(weapon_atr, model_gut, local_ent);
				break;
			case 4:
				apply_knife_model(weapon_atr, model_karambit, local_ent);
				break;
			case 5:
				apply_knife_model(weapon_atr, model_m9, local_ent);
				break;
			case 6:
				apply_knife_model(weapon_atr, model_huntsman, local_ent);
				break;
			case 7:
				apply_knife_model(weapon_atr, model_falchion, local_ent);
				break;
			case 8:
				apply_knife_model(weapon_atr, model_bowie, local_ent);
				break;
			case 9:
				apply_knife_model(weapon_atr, model_butterfly, local_ent);
				break;
			case 10:
				apply_knife_model(weapon_atr, model_shadow_daggers, local_ent);
				break;
			case 11:
				apply_knife_model(weapon_atr, model_ursus, local_ent);
				break;
			case 12:
				apply_knife_model(weapon_atr, model_navaja, local_ent);
				break;
			case 13:
				apply_knife_model(weapon_atr, model_stiletto, local_ent);
				break;
			case 14:
				apply_knife_model(weapon_atr, model_talon, local_ent);
				break;
			case 15:
				apply_knife_model(weapon_atr, model_nomad, local_ent);
				break;
			case 16:
				apply_knife_model(weapon_atr, model_skeleton, local_ent);
				break;
			case 17:
				apply_knife_model(weapon_atr, model_survival, local_ent);
				break;
			case 18:
				apply_knife_model(weapon_atr, model_paracord, local_ent);
				break;
			case 19:
				apply_knife_model(weapon_atr, model_css, local_ent);
				break;
			}
		}
	}
}

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

typedef void(*RecvVarProxyFn)(const c_recv_proxy_data* pData, void* pStruct, void* pOut);
RecvVarProxyFn fnSequenceProxyFn = nullptr;
RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(c_recv_proxy_data* pData, void* pStruct, void* pOut)
{
	static int default_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	static int default_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	static int iBayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	static int iButterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	static int iFlip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	static int iGut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	static int iKarambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	static int iM9Bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	static int iHuntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	static int iFalchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	static int iDagger = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	static int iBowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	static int iUrsus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	static int iNavaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	static int iStiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	static int iTalon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	static int iNomad = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	static int iSkeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	static int iSurvival = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	static int iParacord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	static int iClassic = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");

	if (csgo::local_player)
	{
		if (csgo::local_player->is_alive() && (pData->value.m_int == default_ct
			|| pData->value.m_int == default_t
			|| pData->value.m_int == iGut
			|| pData->value.m_int == iKarambit
			|| pData->value.m_int == iM9Bayonet
			|| pData->value.m_int == iHuntsman
			|| pData->value.m_int == iFalchion
			|| pData->value.m_int == iDagger
			|| pData->value.m_int == iBowie
			|| pData->value.m_int == iButterfly
			|| pData->value.m_int == iFlip
			|| pData->value.m_int == iBayonet
			|| pData->value.m_int == iUrsus
			|| pData->value.m_int == iNavaja
			|| pData->value.m_int == iStiletto
			|| pData->value.m_int == iTalon
			|| pData->value.m_int == iNomad
			|| pData->value.m_int == iSkeleton
			|| pData->value.m_int == iSurvival
			|| pData->value.m_int == iParacord
			|| pData->value.m_int == iClassic))
		{
			if (variables::knife_model == 1)
				pData->value.m_int = iBayonet;
			else if (variables::knife_model == 2)
				pData->value.m_int = iFlip;
			else if (variables::knife_model == 3)
				pData->value.m_int = iGut;
			else if (variables::knife_model == 4)
				pData->value.m_int = iKarambit;
			else if (variables::knife_model == 5)
				pData->value.m_int = iM9Bayonet;
			else if (variables::knife_model == 6)
				pData->value.m_int = iHuntsman;
			else if (variables::knife_model == 7)
				pData->value.m_int = iFalchion;
			else if (variables::knife_model == 8)
				pData->value.m_int = iBowie;
			else if (variables::knife_model == 9)
				pData->value.m_int = iButterfly;
			else if (variables::knife_model == 10)
				pData->value.m_int = iDagger;
			else if (variables::knife_model == 11)
				pData->value.m_int = iUrsus;
			else if (variables::knife_model == 12)
				pData->value.m_int = iNavaja;
			else if (variables::knife_model == 13)
				pData->value.m_int = iStiletto;
			else if (variables::knife_model == 14)
				pData->value.m_int = iTalon;
			else if (variables::knife_model == 15)
				pData->value.m_int = iNomad;
			else if (variables::knife_model == 16)
				pData->value.m_int = iSkeleton;
			else if (variables::knife_model == 17)
				pData->value.m_int = iSurvival;
			else if (variables::knife_model == 18)
				pData->value.m_int = iParacord;
			else if (variables::knife_model == 19)
				pData->value.m_int = iClassic;


		}
	}


	oRecvnModelIndex(pData, pStruct, pOut);
}


void SetViewModelSequence2(const c_recv_proxy_data* pDataConst, void* pStruct, void* pOut)
{
	c_recv_proxy_data* pData = const_cast<c_recv_proxy_data*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	base_view_model_t* pViewModel = (base_view_model_t*)pStruct;

	if (pViewModel) {
		auto pOwner = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(pViewModel->owner()));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->index() == interfaces::engine->get_local_player()) {
			// Get the filename of the current view model.
			const model_t* pModel = interfaces::model_info->get_model(pViewModel->model_index());
			const char* szModel = interfaces::model_info->get_model_name(pModel);

			// Store the current sequence.
			int m_nSequence = pData->value.m_int;

			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_ursus.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_stiletto.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_widowmaker.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_css.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_cord.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_canis.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_outdoor.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_skeleton.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
				}
			}
			// Set the fixed sequence.
			pData->value.m_int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	fnSequenceProxyFn(pData, pStruct, pOut);
}

void skins::animfix_hook()
{
	for (c_client_class* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->proxy_fn);

				// Replace the proxy function with our sequence changer.
				pProp->proxy_fn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}
			break;
		}
	}
}

void skins::animfix_unhook()
{
	for (c_client_class* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;

				// Replace the proxy function with our sequence changer.
				pProp->proxy_fn = fnSequenceProxyFn;

				break;
			}
			break;
		}
	}
}