Name:           messenger-app
Version:        1.0.0
Release:        1%{?dist}
Summary:        Messenger application with SQLite database

License:        MIT
URL:            https://github.com/yourname/messenger_app
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++, qt6-qtbase-devel, sqlite-devel
Requires:       qt6-qtbase, sqlite

%description
Simple C++ Qt application for message management.
Features: add, search, delete, update messages.

%prep
%setup -q

%build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib64/qt6
make

%install
mkdir -p %{buildroot}/usr/bin
cp build/messenger_qt %{buildroot}/usr/bin/messenger-app

%files
%doc README.md
/usr/bin/messenger-app

%changelog
* Mon Apr 13 2026 Student <student@example.com> - 1.0.0-1
- Initial release