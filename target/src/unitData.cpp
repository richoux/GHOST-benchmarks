#include <typeinfo>

#include "unitData.hpp"

UnitData::UnitData( string name,
		    double hp,
		    int armor,
		    Size size,
		    int can_shoot_in,
		    int cooldown,
		    int damage,
		    DamageType damage_type,
		    Range range,
		    Splash splash_radius,
		    Coord coord,
		    bool do_splash,
		    bool do_linear_splash )
  : name(name),
    hp(hp),
    initial_HP(hp),
    armor(armor),
    size(size),
    can_shoot_in(can_shoot_in),
    cooldown(cooldown),
    damage(damage),
    damage_type(damage_type),
    range(range),
    splash_radius(splash_radius),
    coord(coord),
    do_splash(do_splash),
    do_linear_splash(do_linear_splash)
{ }

UnitData::UnitData( const UnitData &other )
  : name(other.name),
    hp(other.hp),
    initial_HP(other.initial_HP),
    armor(other.armor),
    size(other.size),
    can_shoot_in(other.can_shoot_in),
    cooldown(other.cooldown),
    damage(other.damage),
    damage_type(other.damage_type),
    range(other.range),
    splash_radius(other.splash_radius),
    coord(other.coord),
    do_splash(other.do_splash),
    do_linear_splash(other.do_linear_splash)
{ }

UnitData& UnitData::operator=( UnitData other )
{
  this->swap( other );
  return *this;
}

void UnitData::swap( UnitData &other )
{
  std::swap(this->name, other.name);
  std::swap(this->hp, other.hp);
  std::swap(this->initial_HP, other.initial_HP);
  std::swap(this->armor, other.armor);
  std::swap(this->size, other.size);
  std::swap(this->can_shoot_in, other.can_shoot_in);
  std::swap(this->cooldown, other.cooldown);
  std::swap(this->damage, other.damage);
  std::swap(this->damage_type, other.damage_type);
  std::swap(this->range, other.range);
  std::swap(this->splash_radius, other.splash_radius);
  std::swap(this->coord, other.coord);
  std::swap(this->do_splash, other.do_splash);
  std::swap(this->do_linear_splash, other.do_linear_splash);
}

ostream& operator<<( ostream &os, const UnitData &u )
{
  os
    << "Name: " << u.name << "\n"
    << "Coordinates: (" << u.coord.x << ", " << u.coord.y << ")\n"
    << "Can shoot in: " << u.can_shoot_in << " frames" << "\n"
    << "Cooldown: " <<  u.cooldown << "\n"
    << "HP: " <<  u.hp << "\n"
    << "Inital HP: " <<  u.hp << "\n"
    << "Damage: " <<  u.damage << "\n"
    << "Armor: " <<  u.armor << "\n";
  // << "-------" << "\n";
    
  return os;
}
