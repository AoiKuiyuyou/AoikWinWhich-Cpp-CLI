//
#include "stdafx.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

//
bool contain (List<String^>^ item_s, String^ item) {
		for each (String^ itemx in item_s) {
			if (itemx->Equals(item)) {
				return true;
			}
		}

		return false;
}

List<String^>^ uniq (List<String^>^ item_s) {
	List<String^>^ item_s_new = gcnew List<String^>();
	
	for each (String^ item in item_s) {
		if (!contain(item_s_new, item)) {
			item_s_new->Add(item);
		}
	}

	return item_s_new;
}

List<String^>^ find_executable (String^ prog) {
    // 8f1kRCu
    String^ env_var_PATHEXT = Environment::GetEnvironmentVariable("PATHEXT");
    /// can be nullptr
	
    // 6qhHTHF
    // split into a list of extensions
    List<String^>^ ext_s = (env_var_PATHEXT == nullptr)
        ? gcnew List<String^>()
        : gcnew List<String^>(env_var_PATHEXT->Split(Path::PathSeparator));
	
    // 2pGJrMW
    // strip
	List<String^>^ ext_s_old = ext_s;

	ext_s = gcnew List<String^>();

	for each (String^ ext in ext_s_old) {
		ext_s->Add(ext->Trim());
	}
	
    // 2gqeHHl
    // remove empty
	ext_s_old = ext_s;

	ext_s = gcnew List<String^>();

	for each (String^ ext in ext_s_old) {
		if (!ext->Equals("")) {
			ext_s->Add(ext);
		}
	}
	
    // 2zdGM8W
    // convert to lowercase
	ext_s_old = ext_s;

	ext_s = gcnew List<String^>();

	for each (String^ ext in ext_s_old) {
		ext_s->Add(ext->ToLower());
	}

    // 2fT8aRB
    // uniquify
	ext_s = uniq(ext_s);
	
    // 4ysaQVN
    String^ env_var_PATH = Environment::GetEnvironmentVariable("PATH");
    /// can be nullptr
	
    List<String^>^ dir_path_s = (env_var_PATH == nullptr)
        ? gcnew List<String^>()
        : gcnew List<String^>(env_var_PATH->Split(Path::PathSeparator));
	
    // 5rT49zI
    // insert empty dir path to the beginning
    //
    // Empty dir handles the case that |prog| is a path, either relative or
    //  absolute. See code 7rO7NIN.
    dir_path_s->Insert(0, "");
	
    // 2klTv20
    // uniquify
    dir_path_s = uniq(dir_path_s);
	
    //
    String^ prog_lc = prog->ToLower();
	
    bool prog_has_ext = false;
	
	for each (String^ ext in ext_s) {
		if (prog_lc->EndsWith(ext)) {
			prog_has_ext = true;
		}
	}
	
    // 6bFwhbv
    List<String^>^ exe_path_s = gcnew List<String^>();
	
	for each (String^ dir_path in dir_path_s) {
        // 7rO7NIN
        // synthesize a path with the dir and prog
        String^ path = (dir_path->Equals(""))
            ? prog
            : Path::Combine(dir_path, prog);

        // 6kZa5cq
        // assume the path has extension, check if it is an executable
        if (prog_has_ext && File::Exists(path))
        {
            exe_path_s->Add(path);
        }
		
        // 2sJhhEV
        // assume the path has no extension
		for each (String^ ext in ext_s) {
            // 6k9X6GP
            // synthesize a new path with the path and the executable extension
            String^ path_plus_ext = path + ext;

            // 6kabzQg
            // check if it is an executable
			if (File::Exists(path_plus_ext))
			{
				exe_path_s->Add(path_plus_ext);
			}
		}
	}

    // 8swW6Av
    // uniquify
    exe_path_s = uniq(exe_path_s);

	//
	return exe_path_s;
}

int main(array<String ^> ^args)
{
    // 9mlJlKg
    if (args->Length != 1)
    {
        // 7rOUXFo
        // print program usage
        Console::WriteLine("Usage: aoikwinwhich PROG");
        Console::WriteLine("");
        Console::WriteLine("#/ PROG can be either name or path");
        Console::WriteLine("aoikwinwhich notepad.exe");
        Console::WriteLine("aoikwinwhich C:\\Windows\\notepad.exe");
        Console::WriteLine("");
        Console::WriteLine("#/ PROG can be either absolute or relative");
        Console::WriteLine("aoikwinwhich C:\\Windows\\notepad.exe");
        Console::WriteLine("aoikwinwhich Windows\\notepad.exe");
        Console::WriteLine("");
        Console::WriteLine("#/ PROG can be either with or without extension");
        Console::WriteLine("aoikwinwhich notepad.exe");
        Console::WriteLine("aoikwinwhich notepad");
        Console::WriteLine("aoikwinwhich C:\\Windows\\notepad.exe");
        Console::WriteLine("aoikwinwhich C:\\Windows\\notepad");

        // 3nqHnP7
        return 2;
    }

    // 9m5B08H
    // get name or path of a program from cmd arg
    String^ prog = args[0];

    // 8ulvPXM
    // find executables
	List<String^>^ path_s = find_executable(prog);
	
    // 5fWrcaF
    // has found none, exit
    if (path_s->Count == 0)
    {
        // 3uswpx0
        return 1;
    }

	// 9xPCWuS
	// has found some, output
	String^ txt = String::Join("\n", path_s);
	
    Console::WriteLine(txt);
	
    // 4s1yY1b
    return 0;
}
