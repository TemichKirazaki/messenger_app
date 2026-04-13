@echo off
chcp 65001 >nul
echo ========================================
echo    DATABASE TESTS REPORT
echo ========================================
echo.
echo Test Date: %date% %time%
echo.
echo Running tests...
echo.

test_database.exe

echo.
echo ========================================
echo    END OF TEST REPORT
echo ========================================
pause