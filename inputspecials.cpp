// Copyright [2017] [Banana.ch SA - Lugano Switzerland]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "inputspecials.hpp"
#include <QtGlobal>

#if defined Q_OS_WIN

#include "Windows.h"

#include <QVarLengthArray>
#include <QKeyEvent>

bool InputSpecials::isUsingImeKeyboard()
{
   HKL lang_id = GetKeyboardLayout(0); // get the current keyboard layout
   unsigned short prim_lang = LOBYTE(lang_id); //take only the primary language identifier
   switch (prim_lang)
   {
      case 0x04: //Chinese
      case 0x11: //Japanese
      case 0x50: //Mongolian
      case 0x51: //Tibetan
         return true;
      default:
        break;
   }
   return false;

}

void InputSpecials::NotifyToIme(QWidget* /*handle*/, QKeyEvent* keyEvent)
{
   if (!keyEvent)
      return;

   QVarLengthArray<INPUT> inputs;
   KEYBDINPUT ki;
   ki.wVk = keyEvent->nativeVirtualKey();
   ki.time = 0;
   ki.dwFlags = 0;
   ki.dwExtraInfo = 0;
   ki.wScan = keyEvent->text().isEmpty() ? 0 : QChar(keyEvent->text()[0]).unicode();
   ki.dwFlags = KEYEVENTF_UNICODE;
   INPUT input;
   input.type = INPUT_KEYBOARD;
   input.ki = ki;
   inputs.append(input);
   SendInput(inputs.count(), inputs.data(), sizeof(INPUT));
}


#elif defined Q_OS_OSX

#include <QLocale>
#include "cocoabridge.h"
#include <Carbon/Carbon.h>

bool InputSpecials::isUsingImeKeyboard()
{
   TISInputSourceRef source = TISCopyCurrentKeyboardInputSource();
   CFArrayRef array = static_cast<CFArrayRef>(TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages));
   CFStringRef isoCode = static_cast<CFStringRef>(CFArrayGetValueAtIndex(array, 0)); // Actually a RFC3066bis, but it's close enough
   if(isoCode)
   {
      QString iso = QString::fromCFString(isoCode);
      QLocale keyboardInputLocale = QLocale(iso);
      switch (keyboardInputLocale.language())
      {
         case QLocale::Chinese:
         case QLocale::Japanese:
            return true;
         default:
            break;
      }
   }
   return false;
}


void InputSpecials::NotifyToIme(QWidget* /*handle*/, QKeyEvent* /*keyEvent*/)
{
   CocoaBridge::sendCurrentEvent();
}


#elif defined Q_OS_LINUX

#include <QString>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <QApplication>
#include <QWidget>
#include <QKeyEvent>

bool InputSpecials::isUsingImeKeyboard()
{
   //Works on Ubuntu

   QString command = "gsettings get org.gnome.desktop.input-sources current | awk '{ print $NF }'";
   QString result = exec_command(command);
   if(result.isEmpty())
      return false;

   int current = result.toInt();
   current = 2*(current + 1);
   current -= 1;

   command = "gsettings get org.gnome.desktop.input-sources sources | tr -d \"\\',[]()\" " \
             " | awk -v cur=" + QString::number(current) + \
             " '{ print $cur}'";
   result = exec_command(command);
   if(result.startsWith("ibus"))
      return true;
   return false;
}


void InputSpecials::NotifyToIme(QWidget* handle, QKeyEvent* keyEvent)
{
    //this patch in this case does not work
    if(handle && keyEvent)
       QApplication::sendEvent(handle->focusProxy() ? handle->focusProxy() : handle, keyEvent);
}

QString InputSpecials::exec_command(const QString& command)
{
   std::array<char, 128>buffer;
   std::string result;
   std::shared_ptr<FILE> pipe(popen(command.toLatin1().constData(), "r"), pclose);
   if(!pipe)
      return QString();
   while(!feof(pipe.get()))
   {
      if(fgets(buffer.data(), 128, pipe.get())!=NULL)
         result += buffer.data();
   }
   return QString::fromStdString(result);
}

#else //Mobile or any other system

bool InputSpecials::isUsingImeKeyboard()
{
   return false;
}

void InputSpecials::NotifyToIme(QWidget* /*handle*/, QKeyEvent* /*keyEvent*/)
{
}


#endif
