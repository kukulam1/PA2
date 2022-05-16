/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CMap.h"

//TODO
//bomb_time unused
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

class CFlameEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;
};

class CRoundEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;
};

class CShortEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;
};

class CLongEffect : public CEffect
{
    public:
        
        std::unique_ptr<CEffect> Clone ( void ) const override;

        void Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const override;
};