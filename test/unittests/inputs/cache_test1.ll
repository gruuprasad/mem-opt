; ModuleID = 'cache_test1.c'
source_filename = "cache_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test_fn(%struct.packet*, %struct.packet*) #0 {
  %3 = alloca %struct.packet*, align 8
  %4 = alloca %struct.packet*, align 8
  store %struct.packet* %0, %struct.packet** %3, align 8
  store %struct.packet* %1, %struct.packet** %4, align 8
  %5 = load %struct.packet*, %struct.packet** %3, align 8
  %6 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 0
  store i32 1, i32* %6, align 4
  %7 = load %struct.packet*, %struct.packet** %3, align 8
  %8 = getelementptr inbounds %struct.packet, %struct.packet* %7, i32 0, i32 1
  store i32 10, i32* %8, align 4
  %9 = load %struct.packet*, %struct.packet** %3, align 8
  %10 = getelementptr inbounds %struct.packet, %struct.packet* %9, i32 0, i32 2
  store i32 2000, i32* %10, align 4
  %11 = load %struct.packet*, %struct.packet** %3, align 8
  %12 = getelementptr inbounds %struct.packet, %struct.packet* %11, i32 0, i32 3
  store i32 4000, i32* %12, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.packet*, align 8
  %2 = alloca %struct.packet*, align 8
  %3 = call noalias i8* @malloc(i64 16) #2
  %4 = bitcast i8* %3 to %struct.packet*
  store %struct.packet* %4, %struct.packet** %1, align 8
  %5 = call noalias i8* @malloc(i64 16) #2
  %6 = bitcast i8* %5 to %struct.packet*
  store %struct.packet* %6, %struct.packet** %2, align 8
  %7 = load %struct.packet*, %struct.packet** %1, align 8
  %8 = load %struct.packet*, %struct.packet** %2, align 8
  call void @test_fn(%struct.packet* %7, %struct.packet* %8)
  %9 = load %struct.packet*, %struct.packet** %1, align 8
  %10 = bitcast %struct.packet* %9 to i8*
  call void @free(i8* %10) #2
  %11 = load %struct.packet*, %struct.packet** %2, align 8
  %12 = bitcast %struct.packet* %11 to i8*
  call void @free(i8* %12) #2
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @free(i8*) #1

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
