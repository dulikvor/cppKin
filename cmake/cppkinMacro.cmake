macro(LinuxPath_ToWinPath LinuxPath ResultingPath)
			string(REPLACE "/" "\\" ${ResultingPath} "${LinuxPath}")
endmacro()