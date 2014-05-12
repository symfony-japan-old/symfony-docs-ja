format
~~~~~~

**データ型**: ``integer`` または ``string`` **デフォルト**: `IntlDateFormatter::MEDIUM`_ ( `widget`_ が ``single_text`` の場合は ``yyyy-MM-dd`` )

ユーザーの入力を適切なフォーマットに変換するために使用され、オプションは ``IntlDateFormatter`` クラスに渡されます。
`widget`_ オプションが ``single_text`` に設定されている場合に重要であり、ユーザがどのようにデータを入力するのかを定義します。
デフォルトでは、フォーマットは現在のユーザーのロケールに基づいて決定さます。これは、 *期待するフォーマットはユーザーごとに異なる* ことを意味します。
文字列として書式を渡してオーバーライドすることができます。

有効なフォーマットの詳細については、 `Date/Time Format Syntax`_ を参照してください。::

    $builder->add('date_created', 'date', array(
        'widget' => 'single_text',
        // これは single_text フォーマットの実際のデフォルトです。
        'format' => 'yyyy-MM-dd',
    ));

.. note::

    HTML5の "date" フィールドとして表示したい場合、 ``single_text`` ウィジェットを ``yyyy-MM-dd`` ( `RFC 3339`_ のフォーマット) のフォーマットで使用しなくてはいけません。
    それは ``single_text`` ウィジェットを使用する場合のデフォルト値です。

.. _`Date/Time Format Syntax`: http://userguide.icu-project.org/formatparse/datetime#TOC-Date-Time-Format-Syntax
.. _`IntlDateFormatter::MEDIUM`: http://www.php.net/manual/en/class.intldateformatter.php#intl.intldateformatter-constants
.. _`RFC 3339`: http://tools.ietf.org/html/rfc3339

.. 2014/05/05 yositani2002 f3e5cff8694793622e3fc184a4b263d857ad4456
