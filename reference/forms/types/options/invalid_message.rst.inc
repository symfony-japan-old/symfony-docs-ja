invalid_message
~~~~~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``This value is not valid``

フィールドに入力されたデータが意味を成さない（すなわち、バリデーション失敗の）場合に使われるバリデーションエラーのメッセージです。

たとえば、時間に変換される :doc:`time</reference/forms/types/time>` フィールドに無意味な文字を入れたときに、または、 ``number`` フィールドに文字列（例、 ``apple`` ）を入れたときにおきます。

通常の（最小の文字数をセットするような）(ビジネスロジック)バリデーションはバリデーションメッセージをバリデーションルール( :ref:`reference<book-validation-constraint-configuration>` )とあわせて設定すべきです。

.. 2013/12/07 yositani200 06095b654359a1c68524083cc6feb410ce33dcaf
