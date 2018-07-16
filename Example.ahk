^j::
/*
LicenseKey := "BKXZW-UCZLO-UTRUO-VRERF"
*/
LicenseKey := "MPDWY-PQAOW-FKSCH-SGAAU"

VarSetCapacity(Result, 32)
Valid := DllCall("AHKIntegration\CheckKey", AStr, LicenseKey, Ptr, &Result, "Cdecl UInt")

KeyFeatures := NumGet(Result, 24, Int64)
ErrorSubsystem := NumGet(Result, 0, Int64)
ErrorReason := NumGet(Result, 8, Int64)
ErrorExtra := NumGet(Result, 16, Int64)

if (Valid)
{
	F1 := KeyFeatures & (1 << 1) != 0
	F2 := KeyFeatures & (1 << 2) != 0
	F3 := KeyFeatures & (1 << 3) != 0
	F4 := KeyFeatures & (1 << 4) != 0
	F5 := KeyFeatures & (1 << 5) != 0
	F6 := KeyFeatures & (1 << 6) != 0
	F7 := KeyFeatures & (1 << 7) != 0
	F8 := KeyFeatures & (1 << 8) != 0

	Text := "The key was valid.`n"
        Text := Text "`nFeature 1: " F1
        Text := Text "`nFeature 2: " F2
        Text := Text "`nFeature 3: " F3
        Text := Text "`nFeature 4: " F4
        Text := Text "`nFeature 5: " F5
        Text := Text "`nFeature 6: " F6
        Text := Text "`nFeature 7: " F7
        Text := Text "`nFeature 8: " F8
	MsgBox, %Text%
	return
}

if (ErrorSubsystem == 1 && (ErrorReason == 6 || ErrorReason == 7 || ErrorReason == 8))
{
	MsgBox, The key was invalid!
	return
}

if (ErrorSubsystem == 1 && ErrorReason == 9)
{
	MsgBox, The key has expired!
	return
}

if (ErrorSubsystem == 4)
{
	MsgBox, Networking error occurred.`nDebug codes: %ErrorSubsystem% %ErrorReason% %ErrorExtra%
	return
}

MsgBox, An error occurred.`nDebug codes: %ErrorSubsystem% %ErrorReason% %ErrorExtra%
return