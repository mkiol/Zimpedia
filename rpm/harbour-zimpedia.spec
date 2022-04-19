Name:       harbour-zimpedia

# >> macros
%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libxapian.*|libzim.*|libzstd.*|libicui18n.*|libicuuc.*|libicudata.*|libuuid.*$
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Zimpedia
Version:    3.0.2
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/mkiol/Zimpedia
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-zimpedia.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(liblzma)
BuildRequires:  pkgconfig(zlib)
BuildRequires:  pkgconfig(qt5embedwidget)
BuildRequires:  desktop-file-utils
BuildRequires:  sailfish-components-webview-qt5
BuildRequires:  sailfish-components-webview-qt5-pickers
BuildRequires:  sailfish-components-webview-qt5-popups
BuildRequires:  sailfish-components-webview-qt5-devel

%description
Offline reader for Wikipedia


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files
