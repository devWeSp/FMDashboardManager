<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="6" />
	<Project>
		<Option title="Debug" />
		<Option pch_mode="2" />
		<Option compiler="gcc" />
		<Build>
			<Target title="Debug">
				<Option output="bin/Debug/coolDashboardManager" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Debug/" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-g" />
				</Compiler>
			</Target>
		</Build>
		<Compiler>
			<Add option="-Wall" />
			<Add option="-fexceptions" />
		</Compiler>
		<Unit filename="src/main.cpp" />
		<Extensions>
			<code_completion>
				<search_path add="src\core" />
				<search_path add="src\file" />
				<search_path add="src\gui" />
				<search_path add="src" />
			</code_completion>
			<envvars />
			<debugger />
		</Extensions>
	</Project>
</CodeBlocks_project_file>
