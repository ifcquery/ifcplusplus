
function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol('PROJECT_PATH');
		var strProjectName = wizard.FindSymbol('PROJECT_NAME');

		selProj = CreateCustomProject(strProjectName, strProjectPath);
		AddConfig(selProj, strProjectName);
		AddFilters(selProj);

		var InfFile = CreateCustomInfFile();
		AddFilesToCustomProj(selProj, strProjectName, strProjectPath, InfFile);
		PchSettings(selProj);
		InfFile.Delete();

		selProj.Object.Save();
	}
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}

function CreateCustomProject(strProjectName, strProjectPath)
{
	try
	{
		var strProjTemplatePath = wizard.FindSymbol('PROJECT_TEMPLATE_PATH');
		var strProjTemplate = '';
		strProjTemplate = strProjTemplatePath + '\\default.vcproj';

		var Solution = dte.Solution;
		var strSolutionName = "";
		if (wizard.FindSymbol("CLOSE_SOLUTION"))
		{
			Solution.Close();
			strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
			if (strSolutionName.length)
			{
				var strSolutionPath = strProjectPath.substr(0, strProjectPath.length - strProjectName.length);
				Solution.Create(strSolutionPath, strSolutionName);
			}
		}

		var strProjectNameWithExt = '';
		strProjectNameWithExt = strProjectName + '.vcproj';

		var oTarget = wizard.FindSymbol("TARGET");
		var prj;
		if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)  // vsWizardAddSubProject
		{
			var prjItem = oTarget.AddFromTemplate(strProjTemplate, strProjectNameWithExt);
			prj = prjItem.SubProject;
		}
		else
		{
			prj = oTarget.AddFromTemplate(strProjTemplate, strProjectPath, strProjectNameWithExt);
		}
		var fxtarget = wizard.FindSymbol("TARGET_FRAMEWORK_VERSION");
		if (fxtarget != null && fxtarget != "")
		{
		    fxtarget = fxtarget.split('.', 2);
		    if (fxtarget.length == 2)
		      prj.Object.TargetFrameworkVersion = parseInt(fxtarget[0]) * 0x10000 + parseInt(fxtarget[1])
		}
		return prj;
	}
	catch(e)
	{
		throw e;
	}
}

function AddFilters(proj)
{
	try
	{
		// Add the folders to your project
		var strSrcFilter = wizard.FindSymbol('SOURCE_FILTER');
		var group = proj.Object.AddFilter('Source Files');
		group.Filter = strSrcFilter;
		// Add the folders to your project
		var group2 = proj.Object.AddFilter('Header Files');
		group2.Filter = "h";
	}
	catch(e)
	{
		throw e;
	}
}

function AddConfig(proj, strProjectName)
{
	try
	{
    var coin3 = wizard.FindSymbol('COIN_VERSION_RADIO_OPTION3');
    var coin4 = wizard.FindSymbol('COIN_VERSION_RADIO_OPTION4');

    var coinversion = "";
    if (coin3) coinversion = "3";
    else if (coin4) coinversion = "4";
    else wizard.ReportError("Unknown Coin version!");

		//Debug configuration
		var configDebug = proj.Object.Configurations('Debug');
		configDebug.IntermediateDirectory = '$(ConfigurationName)';
		configDebug.OutputDirectory = '$(ConfigurationName)';
		    
		var CLToolDebug = configDebug.Tools('VCCLCompilerTool');
		CLToolDebug.AdditionalIncludeDirectories += "$(COINDIR)\\include";
		CLToolDebug.PreprocessorDefinitions += "COIN_DLL;SOWIN_DLL";
		CLToolDebug.DebugInformationFormat = debugOption.debugEnabled;
		CLToolDebug.RuntimeLibrary = runtimeLibraryOption.rtMultiThreadedDebugDLL;
		
		var LinkToolDebug = configDebug.Tools('VCLinkerTool');
    LinkToolDebug.AdditionalLibraryDirectories += "$(COINDIR)\\lib";
    LinkToolDebug.AdditionalDependencies += "coin" + coinversion +"d.lib sowin1d.lib";
    LinkToolDebug.GenerateDebugInformation = true;

    //Release configuration
		configRelease = proj.Object.Configurations('Release');
		configRelease.IntermediateDirectory = '$(ConfigurationName)';
		configRelease.OutputDirectory = '$(ConfigurationName)';

		var CLToolRelease = configRelease.Tools('VCCLCompilerTool');
		CLToolRelease.AdditionalIncludeDirectories += "$(COINDIR)\\include";
		CLToolRelease.PreprocessorDefinitions += "COIN_DLL;SOWIN_DLL";
		CLToolRelease.RuntimeLibrary = runtimeLibraryOption.rtMultiThreadedDLL;

		var LinkToolRelease = configRelease.Tools('VCLinkerTool');
    LinkToolRelease.AdditionalLibraryDirectories += "$(COINDIR)\\lib";
    LinkToolRelease.AdditionalDependencies += "coin" + coinversion +".lib sowin1.lib";
	}
	catch(e)
	{
		throw e;
	}
}

function PchSettings(proj)
{
	// TODO: specify pch settings
}

function DelFile(fso, strWizTempFile)
{
	try
	{
		if (fso.FileExists(strWizTempFile))
		{
			var tmpFile = fso.GetFile(strWizTempFile);
			tmpFile.Delete();
		}
	}
	catch(e)
	{
		throw e;
	}
}

function CreateCustomInfFile()
{
	try
	{
		var fso, TemplatesFolder, TemplateFiles, strTemplate;
		fso = new ActiveXObject('Scripting.FileSystemObject');

		var TemporaryFolder = 2;
		var tfolder = fso.GetSpecialFolder(TemporaryFolder);
		var strTempFolder = tfolder.Drive + '\\' + tfolder.Name;

		var strWizTempFile = strTempFolder + "\\" + fso.GetTempName();

		var strTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
		var strInfFile = strTemplatePath + '\\Templates.inf';
		wizard.RenderTemplate(strInfFile, strWizTempFile);

		var WizTempFile = fso.GetFile(strWizTempFile);
		return WizTempFile;
	}
	catch(e)
	{
		throw e;
	}
}

function GetTargetName(strName, strProjectName)
{
	try
	{
		// TODO: set the name of the rendered file based on the template filename
		var strTarget = strName;

		if (strName == 'readme.txt')
			strTarget = 'ReadMe.txt';

		if (strName == 'sample.txt')
			strTarget = 'Sample.txt';

		return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

function AddFilesToCustomProj(proj, strProjectName, strProjectPath, InfFile)
{
	try
	{
		var projItems = proj.ProjectItems

		var strTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');

		var strTpl = '';
		var strName = '';

		var strTextStream = InfFile.OpenAsTextStream(1, -2);
		while (!strTextStream.AtEndOfStream)
		{
			strTpl = strTextStream.ReadLine();
			if (strTpl != '')
			{
				strName = strTpl;
				var strTarget = GetTargetName(strName, strProjectName);
				var strTemplate = strTemplatePath + '\\' + strTpl;
				var strFile = strProjectPath + '\\' + strTarget;

				var bCopyOnly = false;  //"true" will only copy the file from strTemplate to strTarget without rendering/adding to the project
				var strExt = strName.substr(strName.lastIndexOf("."));
				if(strExt==".bmp" || strExt==".ico" || strExt==".gif" || strExt==".rtf" || strExt==".css")
					bCopyOnly = true;
				wizard.RenderTemplate(strTemplate, strFile, bCopyOnly);
				proj.Object.AddFile(strFile);
			}
		}
		strTextStream.Close();
	}
	catch(e)
	{
		throw e;
	}
}


// SIG // Begin signature block
// SIG // MIIj7gYJKoZIhvcNAQcCoIIj3zCCI9sCAQExCzAJBgUr
// SIG // DgMCGgUAMGcGCisGAQQBgjcCAQSgWTBXMDIGCisGAQQB
// SIG // gjcCAR4wJAIBAQQQEODJBs441BGiowAQS9NQkAIBAAIB
// SIG // AAIBAAIBAAIBADAhMAkGBSsOAwIaBQAEFEn/XSXgnXOk
// SIG // a6nO+kSaAFS6R457oIIe4TCCBBIwggL6oAMCAQICDwDB
// SIG // AIs8PIgR0T72Y+zfQDANBgkqhkiG9w0BAQQFADBwMSsw
// SIG // KQYDVQQLEyJDb3B5cmlnaHQgKGMpIDE5OTcgTWljcm9z
// SIG // b2Z0IENvcnAuMR4wHAYDVQQLExVNaWNyb3NvZnQgQ29y
// SIG // cG9yYXRpb24xITAfBgNVBAMTGE1pY3Jvc29mdCBSb290
// SIG // IEF1dGhvcml0eTAeFw05NzAxMTAwNzAwMDBaFw0yMDEy
// SIG // MzEwNzAwMDBaMHAxKzApBgNVBAsTIkNvcHlyaWdodCAo
// SIG // YykgMTk5NyBNaWNyb3NvZnQgQ29ycC4xHjAcBgNVBAsT
// SIG // FU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEhMB8GA1UEAxMY
// SIG // TWljcm9zb2Z0IFJvb3QgQXV0aG9yaXR5MIIBIjANBgkq
// SIG // hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqQK9wXDmO/JO
// SIG // Gyifl3heMOqiqY0lX/j+lUyjt/6doiA+fFGim6KPYDJr
// SIG // 0UJkee6sdslU2vLrnIYcj5+EZrPFa3piI9YdPN4PAZLo
// SIG // lsS/LWaammgmmdA6LL8MtVgmwUbnCj44liypKDmo7EmD
// SIG // QuOED7uabFVhrIJ8oWAtd0zpmbRkO5pQHDEIJBSfqeeR
// SIG // KxjmPZhjFGBYBWWfHTdSh/en75QCxhvTv1VFs4mAvzrs
// SIG // VJROrv2nem10Tq8YzJYJKCEAV5BgaTe7SxIHPFb/W/uk
// SIG // ZgoIptKBVlfvtjteFoF3BNr2vq6Alf6wzX/WpxpyXDzK
// SIG // vPAIoyIwswaFybMgdxOF3wIDAQABo4GoMIGlMIGiBgNV
// SIG // HQEEgZowgZeAEFvQcO9pcp4jUX4Usk2O/8uhcjBwMSsw
// SIG // KQYDVQQLEyJDb3B5cmlnaHQgKGMpIDE5OTcgTWljcm9z
// SIG // b2Z0IENvcnAuMR4wHAYDVQQLExVNaWNyb3NvZnQgQ29y
// SIG // cG9yYXRpb24xITAfBgNVBAMTGE1pY3Jvc29mdCBSb290
// SIG // IEF1dGhvcml0eYIPAMEAizw8iBHRPvZj7N9AMA0GCSqG
// SIG // SIb3DQEBBAUAA4IBAQCV6AvAjfOXGDXtuAEk2HcR81xg
// SIG // Mp+eC8s+BZGIj8k65iHy8FeTLLWgR8hi7/zXzDs7Wqk2
// SIG // VGn+JG0/ycyq3gV83TGNPZ8QcGq7/hJPGGnA/NBD4xFa
// SIG // IE/qYnuvqhnIKzclLb5loRKKJQ9jo/dUHPkhydYV81Ks
// SIG // bkMyB/2CF/jlZ2wNUfa98VLHvefEMPwgMQmIHZUpGk3V
// SIG // HQKl8YDgA7Rb9LHdyFfuZUnHUlS2tAMoEv+Q1vAIj364
// SIG // l8WrNyzkeuSod+N2oADQaj/B0jaK4EESqDVqG2rbNeHU
// SIG // HATkqEUEyFozOG5NHA1itwqijNPVVD9GzRxVpnDbEjqH
// SIG // k3Wfp9KgMIIEEjCCAvqgAwIBAgIPAMEAizw8iBHRPvZj
// SIG // 7N9AMA0GCSqGSIb3DQEBBAUAMHAxKzApBgNVBAsTIkNv
// SIG // cHlyaWdodCAoYykgMTk5NyBNaWNyb3NvZnQgQ29ycC4x
// SIG // HjAcBgNVBAsTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEh
// SIG // MB8GA1UEAxMYTWljcm9zb2Z0IFJvb3QgQXV0aG9yaXR5
// SIG // MB4XDTk3MDExMDA3MDAwMFoXDTIwMTIzMTA3MDAwMFow
// SIG // cDErMCkGA1UECxMiQ29weXJpZ2h0IChjKSAxOTk3IE1p
// SIG // Y3Jvc29mdCBDb3JwLjEeMBwGA1UECxMVTWljcm9zb2Z0
// SIG // IENvcnBvcmF0aW9uMSEwHwYDVQQDExhNaWNyb3NvZnQg
// SIG // Um9vdCBBdXRob3JpdHkwggEiMA0GCSqGSIb3DQEBAQUA
// SIG // A4IBDwAwggEKAoIBAQCpAr3BcOY78k4bKJ+XeF4w6qKp
// SIG // jSVf+P6VTKO3/p2iID58UaKboo9gMmvRQmR57qx2yVTa
// SIG // 8uuchhyPn4Rms8VremIj1h083g8BkuiWxL8tZpqaaCaZ
// SIG // 0Dosvwy1WCbBRucKPjiWLKkoOajsSYNC44QPu5psVWGs
// SIG // gnyhYC13TOmZtGQ7mlAcMQgkFJ+p55ErGOY9mGMUYFgF
// SIG // ZZ8dN1KH96fvlALGG9O/VUWziYC/OuxUlE6u/ad6bXRO
// SIG // rxjMlgkoIQBXkGBpN7tLEgc8Vv9b+6RmCgim0oFWV++2
// SIG // O14WgXcE2va+roCV/rDNf9anGnJcPMq88AijIjCzBoXJ
// SIG // syB3E4XfAgMBAAGjgagwgaUwgaIGA1UdAQSBmjCBl4AQ
// SIG // W9Bw72lyniNRfhSyTY7/y6FyMHAxKzApBgNVBAsTIkNv
// SIG // cHlyaWdodCAoYykgMTk5NyBNaWNyb3NvZnQgQ29ycC4x
// SIG // HjAcBgNVBAsTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEh
// SIG // MB8GA1UEAxMYTWljcm9zb2Z0IFJvb3QgQXV0aG9yaXR5
// SIG // gg8AwQCLPDyIEdE+9mPs30AwDQYJKoZIhvcNAQEEBQAD
// SIG // ggEBAJXoC8CN85cYNe24ASTYdxHzXGAyn54Lyz4FkYiP
// SIG // yTrmIfLwV5MstaBHyGLv/NfMOztaqTZUaf4kbT/JzKre
// SIG // BXzdMY09nxBwarv+Ek8YacD80EPjEVogT+pie6+qGcgr
// SIG // NyUtvmWhEoolD2Oj91Qc+SHJ1hXzUqxuQzIH/YIX+OVn
// SIG // bA1R9r3xUse958Qw/CAxCYgdlSkaTdUdAqXxgOADtFv0
// SIG // sd3IV+5lScdSVLa0AygS/5DW8AiPfriXxas3LOR65Kh3
// SIG // 43agANBqP8HSNorgQRKoNWobats14dQcBOSoRQTIWjM4
// SIG // bk0cDWK3CqKM09VUP0bNHFWmcNsSOoeTdZ+n0qAwggRg
// SIG // MIIDTKADAgECAgouqxHcUP9cncvAMAkGBSsOAwIdBQAw
// SIG // cDErMCkGA1UECxMiQ29weXJpZ2h0IChjKSAxOTk3IE1p
// SIG // Y3Jvc29mdCBDb3JwLjEeMBwGA1UECxMVTWljcm9zb2Z0
// SIG // IENvcnBvcmF0aW9uMSEwHwYDVQQDExhNaWNyb3NvZnQg
// SIG // Um9vdCBBdXRob3JpdHkwHhcNMDcwODIyMjIzMTAyWhcN
// SIG // MTIwODI1MDcwMDAwWjB5MQswCQYDVQQGEwJVUzETMBEG
// SIG // A1UECBMKV2FzaGluZ3RvbjEQMA4GA1UEBxMHUmVkbW9u
// SIG // ZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9u
// SIG // MSMwIQYDVQQDExpNaWNyb3NvZnQgQ29kZSBTaWduaW5n
// SIG // IFBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC
// SIG // ggEBALd5fdZds0U5qDSsMdr5JTVJd8D7H57HRXHv0Ubo
// SIG // 1IzDa0xSYvSZAsNN2ElsLyQ+Zb/OI7cLSLd/dd1FvaqP
// SIG // DlDFJSvyoOcNIx/RQST6YpnPGUWlk0ofmc2zLyLDSi18
// SIG // b9kVHjuMORA53b0p9GY7LQEy//4nSKa1bAGHnPu6smN/
// SIG // gvlcoIGEhY6w8riUo884plCFFyeHTt0w9gA99Mb5PYG+
// SIG // hu1sOacuNPa0Lq8KfWKReGacmHMNhq/yxPMguU8SjWPL
// SIG // LNkyRRnuu0qWO1BTGM5mUXmqrYfIVj6fglCIbgWxNcF7
// SIG // JL1SZj2ZTswrfjNuhEcG0Z7QSoYCboYApMCH31MCAwEA
// SIG // AaOB+jCB9zATBgNVHSUEDDAKBggrBgEFBQcDAzCBogYD
// SIG // VR0BBIGaMIGXgBBb0HDvaXKeI1F+FLJNjv/LoXIwcDEr
// SIG // MCkGA1UECxMiQ29weXJpZ2h0IChjKSAxOTk3IE1pY3Jv
// SIG // c29mdCBDb3JwLjEeMBwGA1UECxMVTWljcm9zb2Z0IENv
// SIG // cnBvcmF0aW9uMSEwHwYDVQQDExhNaWNyb3NvZnQgUm9v
// SIG // dCBBdXRob3JpdHmCDwDBAIs8PIgR0T72Y+zfQDAPBgNV
// SIG // HRMBAf8EBTADAQH/MB0GA1UdDgQWBBTMHc52AHBbr/Ha
// SIG // xE6aUUQuo0Rj8DALBgNVHQ8EBAMCAYYwCQYFKw4DAh0F
// SIG // AAOCAQEAe6uufkom8s68TnSiWCd0KnWzhv2rTJR4AE3p
// SIG // yusY3GnFDqJ88wJDxsqHzPhTzMKfvVZv8GNEqUQA7pbI
// SIG // mtUcuAufGQ2U19oerSl97+2mc6yP3jmOPZhqvDht0oiv
// SIG // I/3f6dZpCZGIvf7hALs08/d8+RASLgXrKZaTQmsocbc4
// SIG // j+AHDcldaM29gEFrZqi7t7uONMryAxB8evXS4ELfe/7h
// SIG // 4az+9t/VDbNw1pLjT7Y4onwt1D3bNAtiNwKfgWojifZc
// SIG // Y4+wWrs512CMVYQaM/U7mKCCDKJfi7Mst6Gly6vaILa/
// SIG // MBmFIBQNKrxS9EHgXjDjkihph8Fw4vOnq86AQnJ2DjCC
// SIG // BGowggNSoAMCAQICCmEPeE0AAAAAAAMwDQYJKoZIhvcN
// SIG // AQEFBQAweTELMAkGA1UEBhMCVVMxEzARBgNVBAgTCldh
// SIG // c2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNV
// SIG // BAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEjMCEGA1UE
// SIG // AxMaTWljcm9zb2Z0IENvZGUgU2lnbmluZyBQQ0EwHhcN
// SIG // MDcwODIzMDAyMzEzWhcNMDkwMjIzMDAzMzEzWjB0MQsw
// SIG // CQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3RvbjEQ
// SIG // MA4GA1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9z
// SIG // b2Z0IENvcnBvcmF0aW9uMR4wHAYDVQQDExVNaWNyb3Nv
// SIG // ZnQgQ29ycG9yYXRpb24wggEiMA0GCSqGSIb3DQEBAQUA
// SIG // A4IBDwAwggEKAoIBAQCi2wqNz8LBSZvNqjo0rSNZa9ts
// SIG // viEit5TI6q6/xtUmwjIRi7zaXSz7NlYeFSuujw3dFKNu
// SIG // KEx/Fj9BrI1AsUaIDdmBlK2XBtBXRHZc6vH8DuJ/dKMz
// SIG // y3Tl7+NhoX4Dt0X/1T4S1bDKXg3Qe/K3Ew38YGoohXWM
// SIG // t628hegXtJC+9Ra2Yl3tEd867iFbi6+Ac8NF45WJd2Cb
// SIG // 5613wTeNMxQvE9tiya4aqU+YZ63UIDkwceCNZ0bixhz0
// SIG // DVB0QS/oBSRqIWtJsJLEsjnHQqVtXBhKq4/XjoM+eApH
// SIG // 2KSyhCPD4vJ7ZrFKdL0mQUucYRRgTjDIgvPQC3B87lVN
// SIG // d9IIVXaBAgMBAAGjgfgwgfUwDgYDVR0PAQH/BAQDAgbA
// SIG // MB0GA1UdDgQWBBTzIUCOfFH4VEuY5RfXaoM0BS4m6DAT
// SIG // BgNVHSUEDDAKBggrBgEFBQcDAzAfBgNVHSMEGDAWgBTM
// SIG // Hc52AHBbr/HaxE6aUUQuo0Rj8DBEBgNVHR8EPTA7MDmg
// SIG // N6A1hjNodHRwOi8vY3JsLm1pY3Jvc29mdC5jb20vcGtp
// SIG // L2NybC9wcm9kdWN0cy9DU1BDQS5jcmwwSAYIKwYBBQUH
// SIG // AQEEPDA6MDgGCCsGAQUFBzAChixodHRwOi8vd3d3Lm1p
// SIG // Y3Jvc29mdC5jb20vcGtpL2NlcnRzL0NTUENBLmNydDAN
// SIG // BgkqhkiG9w0BAQUFAAOCAQEAQFdvU2eeIIM0AQ7mF0s8
// SIG // revYgX/uDXl0d0+XRxjzABVpfttikKL9Z6Gc5Cgp+lXX
// SIG // mf5Qv14Js7mm7YLzmB5vWfr18eEM04sIPhYXINHAtUVH
// SIG // CCZgVwlLlPAIzLpNbvDiSBIoNYshct9ftq9pEiSU7uk0
// SIG // Cdt+bm+SClLKKkxJqjIshuihzF0mvLw84Fuygwu6NRxP
// SIG // hEVH/7uUoVkHqZbdeL1Xf6WnTszyrZyaQeLLXCQ+3H80
// SIG // R072z8h7neu2yZxjFFOvrZrv17/PoKGrlcp6K4cswMfZ
// SIG // /GwD2r84rfHRXBkXD8D3yoCmEAga3ZAj57ChTD7qsBEm
// SIG // eA7BLLmka8ePPDCCBJ0wggOFoAMCAQICCmFHUroAAAAA
// SIG // AAQwDQYJKoZIhvcNAQEFBQAweTELMAkGA1UEBhMCVVMx
// SIG // EzARBgNVBAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1Jl
// SIG // ZG1vbmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3Jh
// SIG // dGlvbjEjMCEGA1UEAxMaTWljcm9zb2Z0IFRpbWVzdGFt
// SIG // cGluZyBQQ0EwHhcNMDYwOTE2MDE1MzAwWhcNMTEwOTE2
// SIG // MDIwMzAwWjCBpjELMAkGA1UEBhMCVVMxEzARBgNVBAgT
// SIG // Cldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAc
// SIG // BgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEnMCUG
// SIG // A1UECxMebkNpcGhlciBEU0UgRVNOOkQ4QTktQ0ZDQy01
// SIG // NzlDMScwJQYDVQQDEx5NaWNyb3NvZnQgVGltZXN0YW1w
// SIG // aW5nIFNlcnZpY2UwggEiMA0GCSqGSIb3DQEBAQUAA4IB
// SIG // DwAwggEKAoIBAQCbbdyGUegyOzc6liWyz2/uYbVB0hg7
// SIG // Wp14Z7r4H9kIVZKIfuNBU/rsKFT+tdr+cDuVJ0h+Q6Ay
// SIG // LyaBSvICdnfIyan4oiFYfg29Adokxv5EEQU1OgGo6lQK
// SIG // MyyH0n5Bs+gJ2bC+45klprwl7dfTjtv0t20bSQvm08OH
// SIG // bu5GyX/zbevngx6oU0Y/yiR+5nzJLPt5FChFwE82a1Ma
// SIG // p4az5/zhwZ9RCdu8pbv+yocJ9rcyGb7hSlG8vHysLJVq
// SIG // l3PqclehnIuG2Ju9S/wnM8FtMqzgaBjYbjouIkPR+Y/t
// SIG // 8QABDWTAyaPdD/HI6VTKEf/ceCk+HaxYwNvfqtyuZRvT
// SIG // nbxnAgMBAAGjgfgwgfUwHQYDVR0OBBYEFE8YiYrSygB4
// SIG // xuxZDQ/9fMTBIoDeMB8GA1UdIwQYMBaAFG/oTj+XuTSr
// SIG // S4aPvJzqrDtBQ8bQMEQGA1UdHwQ9MDswOaA3oDWGM2h0
// SIG // dHA6Ly9jcmwubWljcm9zb2Z0LmNvbS9wa2kvY3JsL3By
// SIG // b2R1Y3RzL3RzcGNhLmNybDBIBggrBgEFBQcBAQQ8MDow
// SIG // OAYIKwYBBQUHMAKGLGh0dHA6Ly93d3cubWljcm9zb2Z0
// SIG // LmNvbS9wa2kvY2VydHMvdHNwY2EuY3J0MBMGA1UdJQQM
// SIG // MAoGCCsGAQUFBwMIMA4GA1UdDwEB/wQEAwIGwDANBgkq
// SIG // hkiG9w0BAQUFAAOCAQEANyce9YxA4PZlJj5kxJC8PuNX
// SIG // hd1DDUCEZ76HqCra3LQ2IJiOM3wuX+BQe2Ex8xoT3oS9
// SIG // 6mkcWHyzG5PhCCeBRbbUcMoUt1+6V+nUXtA7Q6q3P7ba
// SIG // YYtxz9R91Xtuv7TKWjCR39oKDqM1nyVhTsAydCt6BpRy
// SIG // AKwYnUvlnivFOlSspGDYp/ebf9mpbe1Ea7rc4BL68K2H
// SIG // DJVjCjIeiU7MzH6nN6X+X9hn+kZL0W0dp33SvgL/826C
// SIG // 84d0xGnluXDMS2WjBzWpRJ6EfTlu/hQFvRpQIbU+n/N3
// SIG // HI/Cmp1X4Wl9aeiDzwJvKiK7NzM6cvrWMB2RrfZQGusT
// SIG // 3jrFt1zNszCCBJ0wggOFoAMCAQICCmFHUroAAAAAAAQw
// SIG // DQYJKoZIhvcNAQEFBQAweTELMAkGA1UEBhMCVVMxEzAR
// SIG // BgNVBAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1v
// SIG // bmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlv
// SIG // bjEjMCEGA1UEAxMaTWljcm9zb2Z0IFRpbWVzdGFtcGlu
// SIG // ZyBQQ0EwHhcNMDYwOTE2MDE1MzAwWhcNMTEwOTE2MDIw
// SIG // MzAwWjCBpjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCldh
// SIG // c2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNV
// SIG // BAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEnMCUGA1UE
// SIG // CxMebkNpcGhlciBEU0UgRVNOOkQ4QTktQ0ZDQy01NzlD
// SIG // MScwJQYDVQQDEx5NaWNyb3NvZnQgVGltZXN0YW1waW5n
// SIG // IFNlcnZpY2UwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAw
// SIG // ggEKAoIBAQCbbdyGUegyOzc6liWyz2/uYbVB0hg7Wp14
// SIG // Z7r4H9kIVZKIfuNBU/rsKFT+tdr+cDuVJ0h+Q6AyLyaB
// SIG // SvICdnfIyan4oiFYfg29Adokxv5EEQU1OgGo6lQKMyyH
// SIG // 0n5Bs+gJ2bC+45klprwl7dfTjtv0t20bSQvm08OHbu5G
// SIG // yX/zbevngx6oU0Y/yiR+5nzJLPt5FChFwE82a1Map4az
// SIG // 5/zhwZ9RCdu8pbv+yocJ9rcyGb7hSlG8vHysLJVql3Pq
// SIG // clehnIuG2Ju9S/wnM8FtMqzgaBjYbjouIkPR+Y/t8QAB
// SIG // DWTAyaPdD/HI6VTKEf/ceCk+HaxYwNvfqtyuZRvTnbxn
// SIG // AgMBAAGjgfgwgfUwHQYDVR0OBBYEFE8YiYrSygB4xuxZ
// SIG // DQ/9fMTBIoDeMB8GA1UdIwQYMBaAFG/oTj+XuTSrS4aP
// SIG // vJzqrDtBQ8bQMEQGA1UdHwQ9MDswOaA3oDWGM2h0dHA6
// SIG // Ly9jcmwubWljcm9zb2Z0LmNvbS9wa2kvY3JsL3Byb2R1
// SIG // Y3RzL3RzcGNhLmNybDBIBggrBgEFBQcBAQQ8MDowOAYI
// SIG // KwYBBQUHMAKGLGh0dHA6Ly93d3cubWljcm9zb2Z0LmNv
// SIG // bS9wa2kvY2VydHMvdHNwY2EuY3J0MBMGA1UdJQQMMAoG
// SIG // CCsGAQUFBwMIMA4GA1UdDwEB/wQEAwIGwDANBgkqhkiG
// SIG // 9w0BAQUFAAOCAQEANyce9YxA4PZlJj5kxJC8PuNXhd1D
// SIG // DUCEZ76HqCra3LQ2IJiOM3wuX+BQe2Ex8xoT3oS96mkc
// SIG // WHyzG5PhCCeBRbbUcMoUt1+6V+nUXtA7Q6q3P7baYYtx
// SIG // z9R91Xtuv7TKWjCR39oKDqM1nyVhTsAydCt6BpRyAKwY
// SIG // nUvlnivFOlSspGDYp/ebf9mpbe1Ea7rc4BL68K2HDJVj
// SIG // CjIeiU7MzH6nN6X+X9hn+kZL0W0dp33SvgL/826C84d0
// SIG // xGnluXDMS2WjBzWpRJ6EfTlu/hQFvRpQIbU+n/N3HI/C
// SIG // mp1X4Wl9aeiDzwJvKiK7NzM6cvrWMB2RrfZQGusT3jrF
// SIG // t1zNszCCBJ0wggOFoAMCAQICEGoLmU/AACWrEdtFH1h6
// SIG // Z6IwDQYJKoZIhvcNAQEFBQAwcDErMCkGA1UECxMiQ29w
// SIG // eXJpZ2h0IChjKSAxOTk3IE1pY3Jvc29mdCBDb3JwLjEe
// SIG // MBwGA1UECxMVTWljcm9zb2Z0IENvcnBvcmF0aW9uMSEw
// SIG // HwYDVQQDExhNaWNyb3NvZnQgUm9vdCBBdXRob3JpdHkw
// SIG // HhcNMDYwOTE2MDEwNDQ3WhcNMTkwOTE1MDcwMDAwWjB5
// SIG // MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3Rv
// SIG // bjEQMA4GA1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWlj
// SIG // cm9zb2Z0IENvcnBvcmF0aW9uMSMwIQYDVQQDExpNaWNy
// SIG // b3NvZnQgVGltZXN0YW1waW5nIFBDQTCCASIwDQYJKoZI
// SIG // hvcNAQEBBQADggEPADCCAQoCggEBANw3bvuvyEJKcRjI
// SIG // zkg+U8D6qxS6LDK7Ek9SyIPtPjPZSTGSKLaRZOAfUIS6
// SIG // wkvRfwX473W+i8eo1a5pcGZ4J2botrfvhbnN7qr9EqQL
// SIG // WSIpL89A2VYEG3a1bWRtSlTb3fHev5+Dx4Dff0wCN5T1
// SIG // wJ4IVh5oR83ZwHZcL322JQS0VltqHGP/gHw87tUEJU05
// SIG // d3QHXcJc2IY3LHXJDuoeOQl8dv6dbG564Ow+j5eecQ5f
// SIG // Kk8YYmAyntKDTisiXGhFi94vhBBQsvm1Go1s7iWbE/jL
// SIG // ENeFDvSCdnM2xpV6osxgBuwFsIYzt/iUW4RBhFiFlG6w
// SIG // HyxIzG+cQ+Bq6H8mjmsCAwEAAaOCASgwggEkMBMGA1Ud
// SIG // JQQMMAoGCCsGAQUFBwMIMIGiBgNVHQEEgZowgZeAEFvQ
// SIG // cO9pcp4jUX4Usk2O/8uhcjBwMSswKQYDVQQLEyJDb3B5
// SIG // cmlnaHQgKGMpIDE5OTcgTWljcm9zb2Z0IENvcnAuMR4w
// SIG // HAYDVQQLExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xITAf
// SIG // BgNVBAMTGE1pY3Jvc29mdCBSb290IEF1dGhvcml0eYIP
// SIG // AMEAizw8iBHRPvZj7N9AMBAGCSsGAQQBgjcVAQQDAgEA
// SIG // MB0GA1UdDgQWBBRv6E4/l7k0q0uGj7yc6qw7QUPG0DAZ
// SIG // BgkrBgEEAYI3FAIEDB4KAFMAdQBiAEMAQTALBgNVHQ8E
// SIG // BAMCAYYwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0B
// SIG // AQUFAAOCAQEAlE0RMcJ8ULsRjqFhBwEOjHBFje9zVL0/
// SIG // CQUt/7hRU4Uc7TmRt6NWC96Mtjsb0fusp8m3sVEhG28I
// SIG // aX5rA6IiRu1stG18IrhG04TzjQ++B4o2wet+6XBdRZ+S
// SIG // 0szO3Y7A4b8qzXzsya4y1Ye5y2PENtEYIb923juasxtz
// SIG // niGI2LS0ElSM9JzCZUqaKCacYIoPO8cTZXhIu8+tgzpP
// SIG // sGJY3jDp6Tkd44ny2jmB+RMhjGSAYwYElvKaAkMve0aI
// SIG // uv8C2WX5St7aA3STswVuDMyd3ChhfEjxF5wRITgCHIes
// SIG // BsWWMrjlQMZTPb2pid7oZjeN9CKWnMywd1RROtZyRLIj
// SIG // 9jGCBHkwggR1AgEBMIGHMHkxCzAJBgNVBAYTAlVTMRMw
// SIG // EQYDVQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRt
// SIG // b25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRp
// SIG // b24xIzAhBgNVBAMTGk1pY3Jvc29mdCBDb2RlIFNpZ25p
// SIG // bmcgUENBAgphD3hNAAAAAAADMAkGBSsOAwIaBQCggaQw
// SIG // GQYJKoZIhvcNAQkDMQwGCisGAQQBgjcCAQQwHAYKKwYB
// SIG // BAGCNwIBCzEOMAwGCisGAQQBgjcCARUwIwYJKoZIhvcN
// SIG // AQkEMRYEFM/7AfUROKBbuNpwrVTECSY6dIMbMEQGCisG
// SIG // AQQBgjcCAQwxNjA0oBqAGABjAHUAcwB0AG8AbQB3AGkA
// SIG // egAuAGoAc6EWgBRodHRwOi8vbWljcm9zb2Z0LmNvbTAN
// SIG // BgkqhkiG9w0BAQEFAASCAQB0iOCqr2mADDrYQK4tliJn
// SIG // PidpvxMcUWDVq8VRKWJsuXaFTvHD8SvJvafHrMduCRI5
// SIG // 6vwN9KaVPvNfPq+JB5qHjNMewmg1xGECGPr+sgLOxNpu
// SIG // 5esqM+6wWMQp9jRd70pk0A1PDqhy+jhiu1J65bcjBCwj
// SIG // /GPbVtXR2WZl+8PbnHUzrZblqp5HGIVB3W0kd8N1G1pc
// SIG // TfEqgbNYLdpyCC1Q4yHylqX45F0bQEAKv1T+ZsmMj+a4
// SIG // Bo1fmcCmE1fER8LVfnc9npPEieuTc49TgKwcFajXt21j
// SIG // 3gukiTexKz7T3VIeRs4slsJKCyHeLjRvZu3c0ZxV5sUd
// SIG // yuXWd1WMwXiUoYICHzCCAhsGCSqGSIb3DQEJBjGCAgww
// SIG // ggIIAgEBMIGHMHkxCzAJBgNVBAYTAlVTMRMwEQYDVQQI
// SIG // EwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4w
// SIG // HAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xIzAh
// SIG // BgNVBAMTGk1pY3Jvc29mdCBUaW1lc3RhbXBpbmcgUENB
// SIG // AgphR1K6AAAAAAAEMAcGBSsOAwIaoF0wGAYJKoZIhvcN
// SIG // AQkDMQsGCSqGSIb3DQEHATAcBgkqhkiG9w0BCQUxDxcN
// SIG // MDgwNzMwMjIyOTMyWjAjBgkqhkiG9w0BCQQxFgQUSjc2
// SIG // nhqIjVZ1zMJfh/mcc91RHlYwDQYJKoZIhvcNAQEFBQAE
// SIG // ggEAB9p6m8N0nuBke0FZe9PNuOAVpML4JbiKhNsVnBVf
// SIG // oO9Ei05oyAhF2xeRUuIpniSmbDGB+9t+nQY2aY543Suo
// SIG // c3rCumhw+peu634kY/rcaKlm+78e+quEf6QKDnsJBfZ+
// SIG // wLVsUPE1UpBGDG/FXYCEfjj7EJ9f8GTi5c/6kQEExsFs
// SIG // uNzpiTUmEaJoMrXoiA8KDJtku1rgJuasr+08+Id9dtnB
// SIG // vG35n7L+juOoiVF4i+a/SoTTTzOmEJmt8xiEkS1dAsTf
// SIG // lUnRZ7ddg0fKXS63vWsQj2Xb/10hRtMpIsGVhMa8j6/5
// SIG // 94bTFTaHKCEKaEsQLQJkmx00eBNizzDFNg8GFw==
// SIG // End signature block
