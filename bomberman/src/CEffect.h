/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CMap.h"

class CEffect
{
    public:
        
        virtual std::unique_ptr<CEffect> Clone ( void ) const = 0;

        virtual void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time ) const = 0;
};

class CBasicEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;

};

class CDoubleEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;
};