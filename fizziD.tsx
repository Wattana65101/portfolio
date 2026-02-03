import React from 'react';

/**
 * Fizzi D Background – พื้นหลังสไตล์ gradient mesh / soft orbs
 * ใช้กับ React app: <FizziDBackground /> หรือ <FizziDBackground dark />
 */
export interface FizziDBackgroundProps {
  /** โหมดมืด (สีจะเข้มลง) */
  dark?: boolean;
  /** คลาสเพิ่มเติมสำหรับ wrapper */
  className?: string;
}

export function FizziDBackground({ dark = false, className = '' }: FizziDBackgroundProps) {
  const theme = dark ? 'dark' : 'light';
  return (
    <div
      className={`fizzi-d ${theme} ${className}`.trim()}
      aria-hidden="true"
      style={{ position: 'fixed', inset: 0, zIndex: -2, overflow: 'hidden', pointerEvents: 'none' }}
    >
      <div className="fizzi-d-base" />
      <div className="fizzi-d-orb fizzi-d-orb-1" />
      <div className="fizzi-d-orb fizzi-d-orb-2" />
      <div className="fizzi-d-orb fizzi-d-orb-3" />
      <div className="fizzi-d-orb fizzi-d-orb-4" />
      <div className="fizzi-d-shine" />
    </div>
  );
}

export default FizziDBackground;
