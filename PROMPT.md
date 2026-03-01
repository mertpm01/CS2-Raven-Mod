# CS2 Raven Mod - AI Development Prompt / Yapay Zeka Geliştirme Promptu

> **IMPORTANT NOTICE / ÖNEMLİ UYARI**: This project is for educational and research purposes only. It demonstrates advanced C++ programming, memory management, overlay rendering, and game development concepts. Use responsibly and in accordance with game terms of service.
> 
> **Bu proje yalnızca eğitim ve araştırma amaçlıdır. Gelişmiş C++ programlama, bellek yönetimi, overlay rendering ve oyun geliştirme konseptlerini gösterir. Sorumlu kullanın ve oyun hizmet şartlarına uygun hareket edin.**

---

## Project Overview / Proje Genel Bakış

**English**: CS2 Raven Mod is an educational C++ project demonstrating external overlay techniques, memory reading patterns, and GUI development for game analysis tools.

**Türkçe**: CS2 Raven Mod, harici overlay teknikleri, bellek okuma kalıpları ve oyun analiz araçları için GUI geliştirmeyi gösteren eğitici bir C++ projesidir.

## Key Features / Ana Özellikler

**English**:
- **Overlay System**: Custom transparent window overlay with GDI+ rendering
- **Memory Reading**: External process memory analysis techniques
- **Visual Information Display**: Player information, health bars, distance calculations
- **GUI Menu**: Interactive configuration interface with Turkish localization
- **Input Handling**: Mouse and keyboard event processing

**Türkçe**:
- **Overlay Sistemi**: GDI+ rendering ile özel şeffaf pencere overlay'i
- **Bellek Okuma**: Harici süreç bellek analizi teknikleri
- **Görsel Bilgi Gösterimi**: Oyuncu bilgileri, sağlık çubukları, mesafe hesaplamaları
- **GUI Menü**: Türkçe yerelleştirme ile interaktif yapılandırma arayüzü
- **Girdi İşleme**: Fare ve klavye olay işleme

---

## Technical Stack / Teknik Yığın

**English**:
- **Language**: C++ (C++20 standard)
- **Build System**: MSBuild (Visual Studio 2022)
- **Graphics**: GDI+ for 2D rendering
- **Memory**: Windows API for process interaction
- **Architecture**: x64 Release build

**Türkçe**:
- **Dil**: C++ (C++20 standardı)
- **Derleme Sistemi**: MSBuild (Visual Studio 2022)
- **Grafikler**: 2D rendering için GDI+
- **Bellek**: Süreç etkileşimi için Windows API
- **Mimari**: x64 Release derlemesi

## Project Structure / Proje Yapısı

**English**:
```
memory-external/
├── classes/          # Core systems
│   ├── config.hpp    # Configuration & defaults
│   ├── menu.hpp      # Menu rendering & interaction
│   ├── render.hpp    # Drawing functions
│   └── globals.hpp   # Global variables
├── hacks/
│   ├── hack.hpp      # Main ESP & aimbot logic
│   └── reader.hpp    # Game memory reading
├── memory/
│   └── memory.hpp    # Memory access utilities
└── main.cpp          # Entry point & window management
```

**Türkçe**:
```
memory-external/
├── classes/          # Ana sistemler
│   ├── config.hpp    # Yapılandırma ve varsayılanlar
│   ├── menu.hpp      # Menü rendering ve etkileşim
│   ├── render.hpp    # Çizim fonksiyonları
│   └── globals.hpp   # Global değişkenler
├── hacks/
│   ├── hack.hpp      # Ana ESP ve aimbot mantığı
│   └── reader.hpp    # Oyun bellek okuma
├── memory/
│   └── memory.hpp    # Bellek erişim araçları
└── main.cpp          # Giriş noktası ve pencere yönetimi
```

## Important Configuration Defaults / Önemli Yapılandırma Varsayılanları (config.hpp)

**English**:
```cpp
// All features enabled by default
inline bool aimbot_enabled = true;
inline bool triggerbot_enabled = true;
inline bool show_box_esp = true;
inline bool show_3d_box_esp = true;
inline bool show_skeleton_esp = true;
inline bool show_crosshair = true;
inline bool show_fps = true;
inline bool show_bomb_timer = true;

// Aimbot settings
inline int aimbot_target = 0;  // 0=Head, 1=Chest, 2=Feet
inline float aimbot_fov = 50.0f;
inline float aimbot_smooth = 3.0f;
inline int aimbot_key = VK_XBUTTON2;  // Mouse 5

// Triggerbot settings
inline int triggerbot_key = VK_XBUTTON1;  // Mouse 4
inline int triggerbot_delay = 50;  // ms
```

**Türkçe**:
```cpp
// Tüm özellikler varsayılan olarak aktif
inline bool aimbot_enabled = true;
inline bool triggerbot_enabled = true;
inline bool show_box_esp = true;
inline bool show_3d_box_esp = true;
inline bool show_skeleton_esp = true;
inline bool show_crosshair = true;
inline bool show_fps = true;
inline bool show_bomb_timer = true;

// Aimbot ayarları
inline int aimbot_target = 0;  // 0=Kafa, 1=Göğüs, 2=Ayak
inline float aimbot_fov = 50.0f;
inline float aimbot_smooth = 3.0f;
inline int aimbot_key = VK_XBUTTON2;  // Fare 5

// Triggerbot ayarları
inline int triggerbot_key = VK_XBUTTON1;  // Fare 4
inline int triggerbot_delay = 50;  // ms
```

## Menu Controls / Menü Kontrolleri

**English**:
- **Open/Close**: DELETE key (VK_DELETE)
- **Navigate**: Mouse scroll wheel
- **Interact**: Left mouse click
- **Exit**: END key

**Türkçe**:
- **Aç/Kapat**: DELETE tuşu (VK_DELETE)
- **Gezin**: Fare tekerleği
- **Etkileşim**: Sol fare tıklaması
- **Çıkış**: END tuşu

## Build Command / Derleme Komutu

**English**:
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "cs2-external-esp.sln" `
  /p:Configuration=Release `
  /p:Platform=x64 `
  /m
```

**Türkçe**:
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "cs2-external-esp.sln" `
  /p:Configuration=Release `
  /p:Platform=x64 `
  /m
```

## Output Location / Çıktı Konumu

**English**: `x64/Release/cs2-external-esp.exe`

**Türkçe**: `x64/Release/cs2-external-esp.exe`

## Key Implementation Details / Ana Uygulama Detayları

> **Educational Note / Eğitim Notu**: The following sections describe technical implementations for educational purposes, demonstrating C++ programming patterns, memory management, and game development concepts.
>
> **Aşağıdaki bölümler, C++ programlama kalıpları, bellek yönetimi ve oyun geliştirme konseptlerini gösteren teknik uygulamaları eğitim amaçlı açıklamaktadır.**

### Aimbot (hack.hpp)

**English**:
- **No prediction**: Direct targeting at current head position
- **Fast execution**: No delays, maximum speed
- **Humanizer**: Optional ±0.3 random offset
- **FOV check**: Pixel-based screen distance
- **Wall check**: Uses `is_spotted` flag
- **Smooth aim**: `dx /= config::aimbot_smooth`

**Türkçe**:
- **Tahmin yok**: Mevcut kafa pozisyonuna doğrudan hedefleme
- **Hızlı çalışma**: Gecikme yok, maksimum hız
- **İnsanlaştırıcı**: İsteğe bağlı ±0.3 rastgele ofset
- **FOV kontrolü**: Piksel tabanlı ekran mesafesi
- **Duvar kontrolü**: `is_spotted` bayrağını kullanır
- **Yumuşak nişan**: `dx /= config::aimbot_smooth`

### Triggerbot (hack.hpp)

**English**:
- **Separate from aimbot**: Independent feature
- **30 pixel radius**: Checks if enemy is near crosshair center
- **Delay system**: Configurable ms delay between shots
- **Team check**: Optional teammate protection

**Türkçe**:
- **Aimbot'tan ayrı**: Bağımsız özellik
- **30 piksel yarıçap**: Düşmanın nişangah merkezine yakın olup olmadığını kontrol eder
- **Gecikme sistemi**: Atışlar arası yapılandırılabilir ms gecikmesi
- **Takım kontrolü**: İsteğe bağlı takım arkadaşı koruması

### ESP (hack.hpp)

**English**:
- **2D Box**: Corner-style boxes with health/armor bars
- **3D Box**: World-space cube rendering
- **Skeleton**: Bone connection lines
- **Info**: HP, armor, weapon, distance, money, flash status
- **Visibility control**: All elements hidden when ESP disabled

**Türkçe**:
- **2D Kutu**: Sağlık/zırh çubukları ile köşe tarzı kutular
- **3D Kutu**: Dünya uzayı küp rendering
- **İskelet**: Kemik bağlantı çizgileri
- **Bilgi**: HP, zırh, silah, mesafe, para, flaş durumu
- **Görünürlük kontrolü**: ESP devre dışı olduğunda tüm öğeler gizlenir

### Bomb Timer (hack.hpp)

**English**:
- **40 second countdown**: Standard C4 timer
- **Auto-reset**: Resets when `isC4Planted` becomes false
- **Visual**: Progress bar, color gradient, urgent warning

**Türkçe**:
- **40 saniye geri sayım**: Standart C4 zamanlayıcı
- **Otomatik sıfırlama**: `isC4Planted` false olduğunda sıfırlanır
- **Görsel**: İlerleme çubuğu, renk gradyanı, acil uyarı

### Menu System (menu.hpp)

**English**:
- **Categories**: 4 main sections with scroll support
- **Turkish labels**: All UI text in Turkish
- **Config save/load**: Automatic JSON persistence
- **Mouse interaction**: Click, drag, scroll support

**Türkçe**:
- **Kategoriler**: Kaydırma desteği ile 4 ana bölüm
- **Türkçe etiketler**: Tüm UI metni Türkçe
- **Yapılandırma kaydet/yükle**: Otomatik JSON kalıcılığı
- **Fare etkileşimi**: Tıklama, sürükleme, kaydırma desteği

## Common Modifications / Yaygın Değişiklikler

### Change Menu Key / Menü Tuşunu Değiştir

**English**: In `main.cpp`, line ~220:
```cpp
bool menu_is_pressed = (GetAsyncKeyState(VK_DELETE) & 0x8000) != 0;
```

**Türkçe**: `main.cpp` dosyasında, ~220. satır:
```cpp
bool menu_is_pressed = (GetAsyncKeyState(VK_DELETE) & 0x8000) != 0;
```

### Change Aimbot Speed / Aimbot Hızını Değiştir

**English**: In `config.hpp`:
```cpp
inline float aimbot_smooth = 3.0f;  // Lower = faster (1-10)
```

**Türkçe**: `config.hpp` dosyasında:
```cpp
inline float aimbot_smooth = 3.0f;  // Düşük = hızlı (1-10)
```

### Change Default Target / Varsayılan Hedefi Değiştir

**English**: In `config.hpp`:
```cpp
inline int aimbot_target = 0;  // 0=Head, 1=Chest, 2=Feet
```

**Türkçe**: `config.hpp` dosyasında:
```cpp
inline int aimbot_target = 0;  // 0=Kafa, 1=Göğüs, 2=Ayak
```

### Disable Auto-Enable Features / Otomatik Etkinleştirmeyi Devre Dışı Bırak

**English**: In `config.hpp`, set to `false`:
```cpp
inline bool aimbot_enabled = false;
inline bool triggerbot_enabled = false;
```

**Türkçe**: `config.hpp` dosyasında, `false` olarak ayarla:
```cpp
inline bool aimbot_enabled = false;
inline bool triggerbot_enabled = false;
```

## Compilation Notes / Derleme Notları

**English**:
- Always use Release x64 configuration
- Warnings are normal (type conversions)
- 0 errors required for successful build
- Output exe is ~200KB

**Türkçe**:
- Her zaman Release x64 yapılandırmasını kullanın
- Uyarılar normaldir (tip dönüşümleri)
- Başarılı derleme için 0 hata gereklidir
- Çıktı exe dosyası ~200KB

## Git Workflow / Git İş Akışı

**English**:
```bash
git add .
git commit -m "Description"
git push origin main
```

**Türkçe**:
```bash
git add .
git commit -m "Açıklama"
git push origin main
```

## Turkish Menu Labels Reference / Türkçe Menü Etiketleri Referansı

**English** → **Türkçe**:
- Aimbot = Aimbot
- Triggerbot = Triggerbot
- ESP = ESP
- Diğer = Other/Misc
- Aktif = Active/Enabled
- Kapalı = Closed/Disabled
- Açık = Open/Enabled
- Yumuşaklık = Smoothness
- Hedef = Target
- Kafa = Head
- Göğüs = Chest
- Ayak = Feet

## Performance Optimizations / Performans Optimizasyonları

**English**:
- Frame throttling for grenade prediction
- Early exit checks for dead players
- Cached distance calculations
- Minimal string operations
- Efficient bone rendering

**Türkçe**:
- Bomba tahmini için kare kısıtlama
- Ölü oyuncular için erken çıkış kontrolleri
- Önbelleğe alınmış mesafe hesaplamaları
- Minimal string işlemleri
- Verimli kemik rendering

## Anti-Cheat Considerations / Anti-Cheat Hususları

> **Educational Note / Eğitim Notu**: This section describes technical approaches for educational purposes only.
>
> **Bu bölüm yalnızca eğitim amaçlı teknik yaklaşımları açıklamaktadır.**

**English**:
- External overlay (not injected)
- No game memory modification
- Read-only memory access
- User-mode operations only

**Türkçe**:
- Harici overlay (enjekte edilmez)
- Oyun belleği değişikliği yok
- Sadece okuma bellek erişimi
- Yalnızca kullanıcı modu işlemleri

## Troubleshooting / Sorun Giderme

**English**:
- **Build mismatch**: Update offsets from cs2-dumper
- **ESP not showing**: Check if CS2 window is foreground
- **Aimbot not working**: Verify key bindings (Mouse 5)
- **Menu not opening**: Check DELETE key, try VK_INSERT

**Türkçe**:
- **Derleme uyuşmazlığı**: cs2-dumper'dan offset'leri güncelleyin
- **ESP görünmüyor**: CS2 penceresinin ön planda olup olmadığını kontrol edin
- **Aimbot çalışmıyor**: Tuş atamalarını doğrulayın (Fare 5)
- **Menü açılmıyor**: DELETE tuşunu kontrol edin, VK_INSERT deneyin

## Future Enhancement Ideas / Gelecek Geliştirme Fikirleri

**English**:
- Configurable colors per feature
- Multiple config profiles
- Hotkey customization in menu
- Radar ESP
- Sound ESP
- Spectator list

**Türkçe**:
- Özellik başına yapılandırılabilir renkler
- Çoklu yapılandırma profilleri
- Menüde kısayol tuşu özelleştirme
- Radar ESP
- Ses ESP
- İzleyici listesi

## Credits / Katkılar

**English**:
- Base project structure from cs2-external-esp
- Offsets from cs2-dumper
- Enhanced by AI assistant (Kiro)
- Turkish localization
- Advanced menu system
- Optimized aimbot & ESP

**Türkçe**:
- Temel proje yapısı cs2-external-esp'den
- Offset'ler cs2-dumper'dan
- AI asistanı (Kiro) tarafından geliştirildi
- Türkçe yerelleştirme
- Gelişmiş menü sistemi
- Optimize edilmiş aimbot ve ESP

---
**Last Updated / Son Güncelleme**: March 2026
**Version / Versiyon**: 2.0 (Raven Mod)
**License / Lisans**: MIT
