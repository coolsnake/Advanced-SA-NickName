#pragma once

cell AMX_NATIVE_CALL ASAN_HOOK_ConnectPlayer(AMX* amx, cell* params)
{
	int playerID = params[1];

	char* name = NULL;
	amx_StrParam(amx, params[2], name);

	int name_strlen = strlen(name);
	for (int i = 0; i < name_strlen; i++)
	{
		int sym_str = (int)name[i];
		if (sym_str < 0)
			sym_str &= 0xFF;
		PlayerInfo[playerID].PlayerName[i] = sym_str;
	}
	PlayerInfo[playerID].PlayerName[name_strlen] = EOF;

	PlayerInfo[playerID].Connected = true;
	return 1;
}

cell AMX_NATIVE_CALL ASAN_HOOK_DisconnectPlayer(AMX* amx, cell* params)
{
	int playerID = params[1];
	PlayerInfo[playerID].Connected = false;
	for (int i = 0; i < NickLength_Config.MaxNickLength; i++)
		PlayerInfo[playerID].PlayerName[i] = EOF;
	return 1;
}

cell AMX_NATIVE_CALL ASAN_HOOK_GetPlayerName(AMX *amx, cell *params)
{
	cell *destination = NULL;
	amx_GetAddr(amx, params[1], &destination);

	int len = 0;

	while (destination[len])
	{
		if (destination[len] < 0)
		{
			destination[len] &= 0xFF;
		}
		len++;
	}
	return 1;
}

cell AMX_NATIVE_CALL ASAN_IsValidNickName(AMX *amx, cell *params)
{
	char* name = NULL;
	amx_StrParam(amx, params[1], name);
	if (name == NULL)
		return false;

	int name_strlen = strlen(name);

	if (!std::regex_match(name, ValidNick_Config.RegexTemplate) || name_strlen < NickLength_Config.MinNickLength || name_strlen > NickLength_Config.MaxNickLength || IsMaxRepeatedNicksError(name) || IsIgnoreRepeatedNicksCaseError(name))
		return false;

	if (IsAllowdedToReplaceUnderscoreSymbols() && !IsValidReplaceSpacesRules(name))
		return false;

	return true;
}

cell AMX_NATIVE_CALL ASAN_GetMinNickLength(AMX *amx, cell *params)
{
	return NickLength_Config.MinNickLength;
}

cell AMX_NATIVE_CALL ASAN_GetMaxNickLength(AMX *amx, cell *params)
{
	return NickLength_Config.MaxNickLength;
}
