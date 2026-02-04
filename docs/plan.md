# 2026_chess 개발 계획 (5단계, TDD)

## 범위
- 기본 이동 + 잡기 + 체크까지 포함
- 캐슬링/앙파상/승격/무승부는 제외
- UI 없음, 순수 C++ 도메인 + gtest 기반 테스트

## 단계별 목표
1. 보드/말 모델 + 기본 이동 규칙
   - Board/Position/Piece/MoveRules 구현
   - 기본 이동과 잡기 규칙 테스트
2. 턴/경로 차단/동일 색 점유 금지
   - GameState와 MoveValidator 도입
   - 턴 교대 및 경로 차단 테스트
3. 체크 규칙
   - CheckDetector 추가
   - 자기 왕이 체크 상태가 되는 이동 금지
4. 종료 조건(체크메이트/스테일메이트 구분 없음)
   - 합법적 이동 없음 상태 감지
5. 외부 인터페이스 정리
   - GameController API 확정
   - 문자열 좌표 파싱 포함

## CI
- GitHub Actions에서 CMake + gtest 실행
- `ctest --output-on-failure`로 테스트 결과 확인
