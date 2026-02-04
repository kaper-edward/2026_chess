# 2026_chess

TDD 기반의 체스 도메인 모델 연습 프로젝트입니다. UI 없이 순수 C++ 도메인 모델과 gtest만 사용합니다.

## Build & Test

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## 범위
- 기본 이동 + 잡기 + 체크까지 포함
- 캐슬링/앙파상/승격/무승부는 제외

자세한 단계별 계획은 `docs/plan.md`에 있습니다.
