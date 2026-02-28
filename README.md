# 🎯 CS2 RAVEN MOD - Advanced ESP & Aimbot

**RAVEN MOD** - Counter-Strike 2 için gelişmiş özellikler ve profesyonel menü sistemi ile donatılmış external ESP ve aimbot projesi.

### ⭐ Projeyi beğendiyseniz yıldız vermeyi unutmayın!

## 🎮 Özellikler

### 🔍 ESP (Wallhack)
- **2D Box ESP** - Klasik kutu gösterimi
- **3D Box ESP** - Üç boyutlu kutu gösterimi
- **Skeleton ESP** - İskelet gösterimi
- **Head Tracker** - Kafa takip sistemi
- **Health & Armor Bars** - Can ve zırh çubukları (gradient renkli)
- **Player Info** - İsim, mesafe, silah, para bilgileri
- **Team ESP** - Takım arkadaşlarını göster/gizle

### 🎯 Aimbot
- **Akıllı Hedef Seçimi** - En yakın düşmana otomatik kilitleme
- **Velocity Prediction** - Hareket eden hedeflere önden atış
- **FOV Sistemi** - Ayarlanabilir görüş alanı (0-250)
- **Smooth Aim** - Yumuşak nişan alma (1-10)
- **Humanizer** - İnsan benzeri rastgele sapma
- **Distance-Based Smoothing** - Mesafeye göre otomatik yumuşatma
- **Target Selection** - Kafa, göğüs veya ayak hedefleme
- **Wall Check** - Duvar arkası algılama
- **Deathmatch Mode** - Ölüm maçı modu

### 🔫 Triggerbot
- **Auto Shoot** - Nişangahın üzerinde düşman varsa otomatik ateş
- **Adjustable Delay** - Ayarlanabilir gecikme (0-500ms)
- **Team Check** - Takım arkadaşlarına ateş etme
- **Separate Keybind** - Ayrı tuş ataması (Mouse 4 default)

### 🎨 Diğer Özellikler
- **FPS Counter** - Gerçek zamanlı FPS göstergesi
- **Custom Crosshair** - Özel nişangah
- **Bomb Timer** - Bomba geri sayım (40 saniye, progress bar, uyarılar)
- **Grenade Prediction** - Granat/molotof yörünge tahmini
- **Auto Update** - Otomatik offset güncelleme

## 🎨 Menü Sistemi

### Profesyonel Arayüz
- **Kategorize Menü** - Aimbot, Triggerbot, ESP, Diğer
- **Scroll Sistemi** - Mouse wheel ile kaydırma
- **Sürüklenebilir** - Menüyü istediğiniz yere taşıyın
- **Modern Tasarım** - Cyan/Turkuaz renk teması
- **Toggle Switches** - Açık/kapalı göstergeleri
- **Sliders** - Hassas ayar kaydırıcıları
- **Key Binding** - Tuş atama sistemi
- **Config System** - Ayarları kaydet/yükle (config.json)

### Menü Kontrolleri
- **Sağ Shift** - Menüyü aç/kapat
- **Mouse Wheel** - Yukarı/aşağı kaydır
- **Mouse Click** - Ayarları değiştir
- **Sürükle** - Menüyü taşı
- **END** - Programdan çık

## 📥 Kurulum

1. **Release** bölümünden en son sürümü indirin
2. **cs2-external-esp.exe** dosyasını çalıştırın
3. **CS2** oyununu açın (sıralama önemli değil)
4. ❗ Oyununuzun **Tam Ekran Pencereli** modda olduğundan emin olun

## ⚙️ Offset Güncelleme

### Otomatik Güncelleme
- Menüden **"Otomatik Güncelleme"** seçeneğini açın
- Program her başlatıldığında otomatik olarak güncellenecek

### Manuel Güncelleme
1. `offsets/update_offsets.py` scriptini çalıştırın
2. Veya [cs2-dumper](https://github.com/a2x/cs2-dumper) reposundan manuel olarak güncelleyin

## 🛠️ Geliştirici Talimatları

### Derleme
1. **Visual Studio 2022** kullanın
2. **x64 - Release** konfigürasyonunda derleyin
3. Binary dosyası `x64/Release/` klasöründe olacak

### Gereksinimler
- Windows 10/11
- Visual Studio 2022
- C++20 desteği

## 🎯 Performans Optimizasyonları

- **Erken Sağlık Kontrolü** - Ölü oyuncuları atla
- **Ekran Pozisyon Kontrolü** - Ekran dışı oyuncuları atla
- **Cached Hesaplamalar** - Tekrarlayan hesaplamaları önbelleğe al
- **Frame Throttling** - Granat tahmini için frame sınırlama
- **Stack Allocation** - 3D box için yığın tahsisi
- **Optimized Loops** - Döngü optimizasyonları

## 📸 Ekran Görüntüleri

[Buraya ekran görüntüleri eklenecek]

## ⚠️ Uyarı

Bu proje **sadece eğitim amaçlıdır**. Oyun içinde kullanımı **ban** ile sonuçlanabilir. Kullanımdan doğacak sorumluluk tamamen kullanıcıya aittir.

## 🙏 Teşekkürler

- [IMXNOOBX](https://github.com/IMXNOOBX) - Orijinal CS2 External ESP projesi
- [UnnamedZ03](https://github.com/UnnamedZ03) - Offset ve rehber
- [a2x](https://github.com/a2x) - CS2 Dumper
- UnknownCheats topluluğu

## 📝 Lisans

Bu proje **MIT License** altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

**Made with ❤️ by balyusuf1111-maker**
