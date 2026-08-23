#ifndef BASE_H
#define BASE_H

#include <vec4.hpp>
#include "vartypes.h"

namespace ENGINE {

    class Base
    {
    public:
        Base();
        virtual ~Base();
        virtual void Render();

        virtual void OnStartDrag(int mx, int my);
        virtual void OnDrag(int mx, int my);
        virtual void OnEndDrag(int mx, int my);


        virtual bool intersect(int x, int y);
        virtual void setWidth( int w);
        virtual void setHeight( int h);
        virtual void setSize( int w, int h);

        virtual void setColor(glm::vec4 col);
        virtual void setBackgroundColor(glm::vec4 col);
        virtual void setDisablecolor(glm::vec4 disCol);
        virtual void calcDragBar();

        virtual void disable();
        virtual void enable();
        virtual bool IsDragging();

        virtual sPoint Pos();
        virtual sSize Size();

        virtual int Height();
        virtual int Width();
        virtual int PosX();
        virtual int PosY();

        virtual glm::vec4 color();
        virtual glm::vec4 backgroundColor();
        virtual glm::vec4 disableColor();

        virtual bool IsEnabled();

        void setPos(int x, int y);
    protected:
        sPoint _Pos;
        sSize  _Size;

        glm::vec4 _Color;
        glm::vec4 _BackgroundColor;
        glm::vec4 _DisableColor;
        bool _Enable;
        int _ResX;
        int _ResY;


        // hlpvars for drag
        int distX;
        int distY;
        sRect  interSectHeadline;
        bool   _Dragging;
    };
}

#endif // BASE_H
